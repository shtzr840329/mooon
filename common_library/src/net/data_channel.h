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
 * �ڲ�ͷ�ļ�����Ҫֱ��������
 *
 * 20110206: ������complete��Ϊfull
 */
#ifndef DATA_CHANNEL_H
#define DATA_CHANNEL_H
#include "net/net_config.h"
#include "sys/syscall_exception.h"
NET_NAMESPACE_BEGIN

class CDataChannel
{
public:
    CDataChannel();
    void attach(int fd);
    
    /** ����SOCKET����
      * @buffer: ���ջ�����
      * @buffer_size: ���ջ�������С
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
    
    ssize_t readv(const struct iovec *iov, int iovcnt);
    ssize_t writev(const struct iovec *iov, int iovcnt);

private:
    int _fd;
};

NET_NAMESPACE_END
#endif // DATA_CHANNEL_H
