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
 * Author: Jian Yi, eyjian@qq.com or eyjian@gmail.com
 *
 * 20110206: ������complete��Ϊfull
 */
#ifndef NET_TCP_CLIENT_H
#define NET_TCP_CLIENT_H
#include "net/ip_node.h"
#include "net/epollable.h"
NET_NAMESPACE_BEGIN

/***
  * TCP�ͻ����࣬�ṩ�ͻ��˵ĸ��ֹ���
  */
class CTcpClient: public CEpollable
{
public:
	CTcpClient();
	~CTcpClient();

    /** �Ƿ�ΪIPV6���� */
    bool is_ipv6() const;

    /** �õ��Զ˶˿ں� */
    uint16_t get_peer_port() const;

    /** �õ��Զ�IP��ַ */
    const ip_address_t& get_peer_ip() const;

    /** ���öԶ˵�IP�Ͷ˿ں� */
    void set_peer(const ipv4_node_t& ip_node);
    void set_peer(const ipv6_node_t& ip_node);

    /***
      * ���öԶ�IP��ַ
      * @ip: �Զ˵�IP��ַ������ΪIPV4��Ҳ����ΪIPV6��ַ
      */
    void set_peer_ip(const ip_address_t& ip);

    /** ���öԶ˶˿ں� */
	void set_peer_port(uint16_t port) { _peer_port = port; }

    /** �������ӵ�����ĳ�ʱ������ */
	void set_connect_timeout_milliseconds(uint32_t milli_seconds) { _milli_seconds = milli_seconds; }

    /***
      * �첽���ӣ������Ƿ����������ӳɹ���������������
      * @return: ������ӳɹ����򷵻�true����������������ӹ����У��򷵻�false
      * @exception: ���Ӵ����׳�CSyscallException�쳣
      */
    bool async_connect();

    /***
      * ��ʱ����
      * ��������������ӳɹ�����ȴ���set_connect_timeout_millisecondsָ����ʱ����
      * ��������ʱ������δ���ӳɹ�������������
      * @exception: ���ӳ����ʱ���׳�CSyscallException�쳣
      */
    void timed_connect();
    
    /** ����SOCKET����
      * @buffer: ���ջ�����
      * @buffer_size: ���ջ������ֽ���
      * @return: ����յ����ݣ��򷵻��յ����ֽ���������Զ˹ر������ӣ��򷵻�0��
      *          ���ڷ��������ӣ���������ݿɽ��գ��򷵻�-1
      * @exception: ���Ӵ����׳�CSyscallException�쳣
      */
    ssize_t receive(char* buffer, size_t buffer_size);

    /** ����SOCKET����
      * @buffer: ���ͻ�����
      * @buffer_size: ��Ҫ���͵��ֽڴ�С
      * @return: ������ͳɹ����򷵻�ʵ�ʷ��͵��ֽ��������ڷ����������ӣ�������ܼ������ͣ��򷵻�-1
      * @exception: �����������������׳�CSyscallException�쳣
      */
    ssize_t send(const char* buffer, size_t buffer_size);

    /** �������գ�����ɹ����أ���һ��������ָ���ֽ���������
      * @buffer: ���ջ�����
      * @buffer_size: ���ջ������ֽڴ�С������ʵ���Ѿ����յ����ֽ���(���ܳɹ�����ʧ�ܻ��쳣)
      * @return: ����ɹ����򷵻�true������������ӱ��Զ˹ر��򷵻�false
      * @exception: �����������������׳�CSyscallException�����ڷ��������ӣ�Ҳ�����׳�CSyscallException�쳣
      */
    bool full_receive(char* buffer, size_t& buffer_size);

    /** �������ͣ�����ɹ����أ������Ƿ�����ָ���ֽ���������
      * @buffer: ���ͻ�����
      * @buffer_size: ��Ҫ���͵��ֽ���������ʵ���Ѿ������˵��ֽ���(���ܳɹ�����ʧ�ܻ��쳣)
      * @return: �޷���ֵ
      * @exception: �������������׳�CSyscallException�쳣�����ڷ��������ӣ�Ҳ�����׳�CSyscallException�쳣
      * @ע�������֤������0�ֽڵ����ݣ�Ҳ����buffer_size�������0
      */
    void full_send(const char* buffer, size_t& buffer_size);

    /** �����ļ��������߱��뱣֤offset+count�������ļ���С
      * @file_fd: �򿪵��ļ����
      * @offset: �ļ�ƫ��λ�ã�����ɹ��򷵻��µ�ƫ��λ��
      * @count: ��Ҫ���͵Ĵ�С
      */
    ssize_t send_file(int file_fd, off_t *offset, size_t count);
    void full_send_file(int file_fd, off_t *offset, size_t& count);

    /** �����ڴ�ӳ��ķ�ʽ���գ��������ݴ���ļ����ʺ��ļ�����̫��
      * @file_fd: �򿪵��ļ����
      * @size: ��Ҫд���ļ��Ĵ�С������ʵ���Ѿ����յ����ֽ���(���ܳɹ�����ʧ�ܻ��쳣)
      * @offset: д���ļ���ƫ��ֵ
      * @return: ������ӱ��Զ˹رգ��򷵻�false����ɹ�����true
      * @exception: �������ϵͳ���ô������׳�CSyscallException�쳣
      */
    bool full_receive_tofile_bymmap(int file_fd, size_t& size, size_t offset);

    /** ����write���õķ�ʽ���գ��������ݴ���ļ����ʺ������С���ļ������Ǵ��ļ��ᵼ�¸õ��ó�ʱ������
      * @file_fd: �򿪵��ļ����
      * @size: ��Ҫд���ļ��Ĵ�С������ʵ���Ѿ����յ����ֽ���(���ܳɹ�����ʧ�ܻ��쳣)
      * @offset: д���ļ���ƫ��ֵ
      * @return: ������ӱ��Զ˹رգ��򷵻�false����ɹ�����true
      * @exception: �������ϵͳ���ô������׳�CSyscallException�쳣
      */
    bool full_receive_tofile_bywrite(int file_fd, size_t& size, size_t offset);
    
    /***
      * һ���Զ�һ�����ݣ���ϵͳ����readv���÷���ͬ
      * @return: ����ʵ�ʶ�ȡ�����ֽ���
      * @exception: �������ϵͳ���ô������׳�CSyscallException�쳣 
      */
    ssize_t readv(const struct iovec *iov, int iovcnt);

    /***
      * һ����дһ�����ݣ���ϵͳ����writev���÷���ͬ
      * @return: ����ʵ��д����ֽ���
      * @exception: �������ϵͳ���ô������׳�CSyscallException�쳣 
      */
    ssize_t writev(const struct iovec *iov, int iovcnt);

    /** �ж������Ƿ��Ѿ�����
      * @return: ��������Ѿ��������򷵻�true�����򷵻�false
      */
	bool is_connect_established() const;
    bool is_connect_establishing() const;

    /** ����Ϊ�Ѿ�����״̬���������첽���ӣ��������ʲô������
      * async_connect���ܷ�������������״̬�������ӳɹ�����Ҫ���ô˺��������ó��Ѿ�����״̬�������ڵ���close֮ǰ
      * ��һֱ����������״̬֮��
      */
    void set_connected_state();   
    
    /** �õ���ǰ�Ѿ������������Ӵ��� */
    volatile uint32_t get_reconnect_times() const;

public: // override
    /** �ر����� */
	virtual void close();

private:
    /** ���ӳɹ�֮�󱻵��� */
    virtual void after_connect();
    /** connect֮ǰ�����ã�����(Ҳ�ɲ�)��д�÷���������connectǰ��һЩ���������޸���Ҫ���ӵ�IP�� */
    virtual bool before_connect();  
    /** ����ʧ�ܣ�������ʧ��ʱ�ᱻ���� */
    virtual void connect_failure();

private:
    bool do_connect(int& fd, bool nonblock);    
    
private:
    uint16_t _peer_port;        /** ���ӵĶԶ˶˿ں� */
    ip_address_t _peer_ip;      /** ���ӵĶԶ�IP��ַ */	
	uint32_t _milli_seconds;    /** ���ӳ�ʱ�ĺ����� */
    void* _data_channel;
	uint8_t _connect_state;     /** ����״̬��1: �Ѿ�������2: ���ڽ������ӣ�0: δ���� */
    atomic_t _reconnect_times;  /** ��ǰ�Ѿ������������Ӵ��� */
};

NET_NAMESPACE_END
#endif // NET_TCP_CLIENT_H
