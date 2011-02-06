/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: jian yi, eyjian@qq.com
 */
#include "net/net_util.h"
#include "net/tcp_client.h"
#include "net/data_channel.h"
#define CONNECT_UNESTABLISHED 0
#define CONNECT_ESTABLISHED   1
#define CONNECT_ESTABLISHING  2
NET_NAMESPACE_BEGIN

CTcpClient::CTcpClient()
	:_peer_port(0)
	,_milli_seconds(0)
	,_connect_state(CONNECT_UNESTABLISHED)
{
	_data_channel = new CDataChannel;
    atomic_set(&_reconnect_times, 0);
}

CTcpClient::~CTcpClient()
{
	delete (CDataChannel *)_data_channel;
}

bool CTcpClient::is_ipv6() const
{
    return _peer_ip.is_ipv6();
}

uint16_t CTcpClient::get_peer_port() const
{
    return _peer_port;
}

const ip_address_t& CTcpClient::get_peer_ip() const
{
    return _peer_ip;
}

void CTcpClient::set_peer(const ipv4_node_t& ip_node)
{    
    _peer_ip = ip_node.ip;
    _peer_port = ip_node.port;
}

void CTcpClient::set_peer(const ipv6_node_t& ip_node)
{
    _peer_ip = (uint32_t*)ip_node.ip;
    _peer_port = ip_node.port;
}

void CTcpClient::set_peer_ip(const ip_address_t& ip)
{
    _peer_ip = ip;
}

void CTcpClient::close()
{	    
    if (is_connect_establishing())
    {
        // ������before_close()
        if (CEpollable::do_close())
            connect_failure();
    }
    else
    {
        // �����before_close()
	    CEpollable::close();        
    }

    _connect_state = CONNECT_UNESTABLISHED;
}

void CTcpClient::after_connect()
{
    // �������ѡ��ȥ������
}

bool CTcpClient::before_connect()
{
    // �������ѡ��ȥ������
    return true;
}

void CTcpClient::connect_failure()
{
    // �������ѡ��ȥ������
}

bool CTcpClient::do_connect(int& fd, bool nonblock)
{   
    // ����������֮ǰ��һЩ����
    if (!before_connect()) return false;
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == fd)
		throw sys::CSyscallException(errno, __FILE__, __LINE__, "socket error");

    if (nonblock)
        net::set_nonblock(fd, true);

    socklen_t addr_length;
    struct sockaddr* peer_addr;
    struct sockaddr_in peer_addr_in;
    struct sockaddr_in6 peer_addr_in6;    
            
    const uint32_t* ip_data = _peer_ip.get_address_data();
    if (_peer_ip.is_ipv6())
    {
        addr_length = sizeof(struct sockaddr_in6);
        peer_addr = (struct sockaddr*)&peer_addr_in6;        
        peer_addr_in6.sin6_family = AF_INET6;
        peer_addr_in6.sin6_port = htons(_peer_port);
        memcpy(&peer_addr_in6.sin6_addr, ip_data, addr_length);
    }
    else
    {
        addr_length = sizeof(struct sockaddr_in);
        peer_addr = (struct sockaddr*)&peer_addr_in;
	    peer_addr_in.sin_family = AF_INET;
        peer_addr_in.sin_port = htons(_peer_port);
        peer_addr_in.sin_addr.s_addr = ip_data[0];
        memset(peer_addr_in.sin_zero, 0, sizeof(peer_addr_in.sin_zero));
    }	
    
    return (0 == connect(fd, peer_addr, addr_length)) || (EISCONN == errno);
}

bool CTcpClient::is_connect_established() const 
{ 
    return CONNECT_ESTABLISHED == _connect_state; 
}

bool CTcpClient::is_connect_establishing() const 
{ 
    return CONNECT_ESTABLISHING == _connect_state; 
}

void CTcpClient::set_connected_state()
{
    if (CONNECT_ESTABLISHING == _connect_state)
    {
        _connect_state = CONNECT_ESTABLISHED;
        atomic_set(&_reconnect_times, 0); // һ�����ӳɹ����ͽ������Ӵ�������
        after_connect();
    }
}

volatile uint32_t CTcpClient::get_reconnect_times() const
{
    return atomic_read(&_reconnect_times);
}

bool CTcpClient::async_connect()
{
    int fd = -1;    
    atomic_inc(&_reconnect_times); // �����Ӵ�����1��������ӳɹ����1
    
    if (!do_connect(fd, true))
    {
        if (errno != EINPROGRESS)    
        {
            connect_failure(); // ����ʧ��
            throw sys::CSyscallException(errno, __FILE__, __LINE__);
        }
    }
    
    set_fd(fd);
    ((CDataChannel *)_data_channel)->attach(fd);
    _connect_state = (EINPROGRESS == errno)? CONNECT_ESTABLISHING: CONNECT_ESTABLISHED;
    
    // �����Ѿ��ɹ�
    if (CONNECT_ESTABLISHED == _connect_state) 
    {
        atomic_set(&_reconnect_times, 0);
        after_connect();
    }

    // ���ӻ��ڽ�����
    return errno != EINPROGRESS;
}

void CTcpClient::timed_connect()
{                 	
    int fd = -1;
    atomic_inc(&_reconnect_times); // �����Ӵ�����1��������ӳɹ����1
    
    do
    {    
	    try
	    {
            // ��ʱ������Ҫ������Ϊ������
            bool nonblock = _milli_seconds > 0;                   
            if (do_connect(fd, nonblock))
                break; // һ�������ӳɹ���
            
            // ���ӳ������ܼ���
            if ((0 == _milli_seconds) || (errno != EINPROGRESS))
                throw sys::CSyscallException(errno, __FILE__, __LINE__);

            // �첽�����У�ʹ��poll��ʱ̽��
            
		    struct pollfd fds[1];
		    fds[0].fd = fd;
		    fds[0].events = POLLIN | POLLOUT | POLLERR;

            if (!CNetUtil::timed_poll(fd, POLLIN | POLLOUT | POLLERR, _milli_seconds))		    
			    throw sys::CSyscallException(ETIMEDOUT, __FILE__, __LINE__);

		    int errcode = 0;
		    socklen_t errcode_length = sizeof(errcode);
		    if (-1 == getsockopt(fd, SOL_SOCKET, SO_ERROR, &errcode, &errcode_length))
			    throw sys::CSyscallException(errno, __FILE__, __LINE__);
		    if (errcode != 0)
			    throw sys::CSyscallException(errcode, __FILE__, __LINE__);

            // �ܹ��ߵ�����϶���_milli_seconds > 0
            net::set_nonblock(fd, false);
	    }
	    catch (sys::CSyscallException& ex)
	    {
            connect_failure(); // ����ʧ��
		    ::close(fd);
		    throw;
	    }
    } while(false);

    // ����fd
    set_fd(fd);
    ((CDataChannel *)_data_channel)->attach(fd);
	_connect_state = CONNECT_ESTABLISHED;
    atomic_set(&_reconnect_times, 0); // һ�����ӳɹ����ͽ������Ӵ�������
    after_connect();
}

ssize_t CTcpClient::receive(char* buffer, size_t buffer_size) 
{ 
	return ((CDataChannel *)_data_channel)->receive(buffer, buffer_size); 
}

ssize_t CTcpClient::send(const char* buffer, size_t buffer_size)
{ 
	return ((CDataChannel *)_data_channel)->send(buffer, buffer_size); 
}

bool CTcpClient::full_receive(char* buffer, size_t& buffer_size) 
{ 
	return ((CDataChannel *)_data_channel)->full_receive(buffer, buffer_size); 
}

void CTcpClient::full_send(const char* buffer, size_t& buffer_size)
{ 
	((CDataChannel *)_data_channel)->full_send(buffer, buffer_size); 
}

ssize_t CTcpClient::send_file(int file_fd, off_t *offset, size_t count)
{
    return ((CDataChannel *)_data_channel)->send_file(file_fd, offset, count); 
}

void CTcpClient::full_send_file(int file_fd, off_t *offset, size_t& count)
{
    ((CDataChannel *)_data_channel)->full_send_file(file_fd, offset, count); 
}

bool CTcpClient::full_receive_tofile_bymmap(int file_fd, size_t& size, size_t offset)
{
    return ((CDataChannel *)_data_channel)->full_receive_tofile_bymmap(file_fd, size, offset); 
}

bool CTcpClient::full_receive_tofile_bywrite(int file_fd, size_t& size, size_t offset)
{
    return ((CDataChannel *)_data_channel)->full_receive_tofile_bywrite(file_fd, size, offset); 
}

ssize_t CTcpClient::readv(const struct iovec *iov, int iovcnt)
{
    return ((CDataChannel *)_data_channel)->readv(iov, iovcnt);
}

ssize_t CTcpClient::writev(const struct iovec *iov, int iovcnt)
{
    return ((CDataChannel *)_data_channel)->writev(iov, iovcnt);
}

NET_NAMESPACE_END
