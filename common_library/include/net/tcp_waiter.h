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
 *
 * 20110206: ������complete��Ϊfull
 */
#ifndef TCP_WAITER_H
#define TCP_WAITER_H
#include <sys/uio.h>
#include "net/epollable.h"
NET_NAMESPACE_BEGIN

/***
  * TCP������࣬�ṩ����˵ĸ��ֹ���
  */
class CTcpWaiter: public CEpollable
{
public:
	CTcpWaiter();
	~CTcpWaiter();

    /** �õ��Զ˵�IP��ַ������attach���߲ſ��� */
    const ip_address_t& get_peer_ip() const { return _peer_ip; }

    /** �õ��Զ˵Ķ˿ںţ�����attach���߲ſ��� */
    port_t get_peer_port() const { return _peer_port; }

    /***
      * ������һ��fd
      * @fd: ���رյ�fd��fdΪCListener::accept�ķ���ֵ
      * @peer_ip: �Զ˵�IP��ַ
      * @peer_port: �Զ˵Ķ˿ں�
      */
    void attach(int fd, const ip_address_t& peer_ip, port_t peer_port);

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
      * @ע�������֤������0�ֽڵ����ݣ�Ҳ����buffer_size�������0
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
      * @buffer_size: ��Ҫ���͵��ֽ���������ʵ���Ѿ��ӷ����˵��ֽ���(���ܳɹ�����ʧ�ܻ��쳣)
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

private:
    void* _data_channel;    
    ip_address_t _peer_ip; /** �Զ˵�IP��ַ */
    port_t _peer_port;     /** �Զ˵Ķ˿ں� */    
};

NET_NAMESPACE_END
#endif // TCP_WAITER_H
