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
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <sys/mmap.h>
#include <sys/atomic.h>
#include <sys/util.h>
#include <net/util.h>
#include "net/data_channel.h"
NET_NAMESPACE_BEGIN

static atomic_t gs_send_file_bytes;
static atomic_t gs_send_buffer_bytes;
static atomic_t gs_recv_buffer_bytes;

// 仅用于静态数据的初始化
static class __X
{
public:
    __X()
    {
        atomic_set(&gs_send_file_bytes, 0);
        atomic_set(&gs_send_buffer_bytes, 0);
        atomic_set(&gs_recv_buffer_bytes, 0);
    }
}__init_bytes;

long get_send_file_bytes()
{
    return atomic_read(&gs_send_file_bytes);
}

long get_send_buffer_bytes()
{
    return atomic_read(&gs_send_buffer_bytes);
}

long get_recv_buffer_bytes()
{
    return atomic_read(&gs_recv_buffer_bytes);
}

//////////////////////////////////////////////////////////////////////////
CDataChannel::CDataChannel()
    :_fd(-1)
{
}

void CDataChannel::attach(int fd)
{
    _fd = fd;
}

ssize_t CDataChannel::receive(char* buffer, size_t buffer_size)
{
    ssize_t retval;

    if (0 == buffer_size)
    {
        throw sys::CSyscallException(EINVAL, __FILE__, __LINE__);
    }
    for (;;)
    {
        retval = ::recv(_fd, buffer, buffer_size, 0);

        if (retval != -1) break;        
        if (EWOULDBLOCK == errno) break;
        if (EINTR == errno) continue;

        throw sys::CSyscallException(errno, __FILE__, __LINE__);        
    }

    atomic_add(retval, &gs_recv_buffer_bytes);
    // if retval is equal 0
    return retval;
}

ssize_t CDataChannel::send(const char* buffer, size_t buffer_size)
{
    ssize_t retval;

    if (0 == buffer_size)
    {
        throw sys::CSyscallException(EINVAL, __FILE__, __LINE__);
    }
    for (;;)
    {
        retval = ::send(_fd, buffer, buffer_size, 0);

        if (retval != -1) break;        
        if (EWOULDBLOCK == errno) break;
        if (EINTR == errno) continue;

        throw sys::CSyscallException(errno, __FILE__, __LINE__);        
    }
    
    atomic_add(retval, &gs_send_buffer_bytes);
    return retval;
}

ssize_t CDataChannel::timed_receive(char* buffer, size_t buffer_size, uint32_t milliseconds)
{
    size_t buffer_offset = 0;

    for (;;)
    {
        if (!CUtil::timed_poll(_fd, POLLIN, milliseconds))
        {
            break;
        }

        ssize_t retval = receive(buffer+buffer_offset, buffer_size-buffer_offset);
        if (0 == retval)
        {
            break;
        }
        if (retval > 0)
        {
            buffer_offset += retval;
            if (buffer_offset == buffer_size)
            {
                break;
            }
        }
    }

    return buffer_offset;
}

ssize_t CDataChannel::timed_send(const char* buffer, size_t buffer_size, uint32_t milliseconds)
{
    size_t buffer_offset = 0;
    
    for (;;)
    {
        if (!CUtil::timed_poll(_fd, POLLOUT, milliseconds))
        {
            break;
        }

        ssize_t retval = send(buffer+buffer_offset, buffer_size-buffer_offset);
        if (retval > 0)
        {
            buffer_offset += retval;
            if (buffer_offset == buffer_size)
            {
                break;
            }
        }
    }

    return buffer_offset;
}

bool CDataChannel::full_receive(char* buffer, size_t& buffer_size)
{    
    char* buffer_offset = buffer;
    size_t remaining_size = buffer_size;

    while (remaining_size > 0)
    {
        ssize_t retval = CDataChannel::receive(buffer_offset, remaining_size);
        if (0 == retval)
        {
            buffer_size = buffer_size - remaining_size;
            return false;
        }
        if (-1 == retval)
        {
            buffer_size = buffer_size - remaining_size;
            throw sys::CSyscallException(errno, __FILE__, __LINE__);
        }

        buffer_offset += retval;
        remaining_size -= retval;        
    }

    buffer_size = buffer_size - remaining_size;
    return true;
}

void CDataChannel::full_send(const char* buffer, size_t& buffer_size)
{    
    const char* buffer_offset = buffer;
    size_t remaining_size = buffer_size;

    while (remaining_size > 0)
    {
        ssize_t retval = CDataChannel::send(buffer_offset, remaining_size);
        if (-1 == retval)
        {
            buffer_size = buffer_size - remaining_size;
            throw sys::CSyscallException(errno, __FILE__, __LINE__);
        }

        buffer_offset += retval;
        remaining_size -= retval;        
    }
    
    buffer_size = buffer_size - remaining_size;
}

ssize_t CDataChannel::send_file(int file_fd, off_t *offset, size_t count)
{
    ssize_t retval;

    for (;;)
    {
        retval = sendfile(_fd, file_fd, offset, count);
        if (retval != -1) break;        
        if (EWOULDBLOCK == errno) break;
        if (EINTR == errno) continue;

        throw sys::CSyscallException(errno, __FILE__, __LINE__); 
    }

    atomic_add(retval, &gs_send_file_bytes);
    return retval;
}

void CDataChannel::full_send_file(int file_fd, off_t *offset, size_t& count)
{    
    size_t remaining_size = count;
    while (remaining_size > 0)
    {
        ssize_t retval = CDataChannel::send_file(file_fd, offset, remaining_size);
        if (-1 == retval)
        {
            count = count - remaining_size;
            throw sys::CSyscallException(errno, __FILE__, __LINE__);
        }

        offset += retval;
        remaining_size -= retval;        
    }

    count = count - remaining_size;
}

bool CDataChannel::full_map_tofile(int file_fd, size_t& size, size_t offset)
{
    sys::mmap_t* ptr;
    
    try
    {
        ptr = sys::CMMap::map_write(file_fd, size, offset);
    }
    catch (...)
    {
        size = 0;
        throw;
    }
    
    try
    {
        sys::MMapHelper mmap_helper(ptr);
        bool retval = CDataChannel::full_receive((char*)ptr->addr, ptr->len);
        size = ptr->len;
        return retval;
    }
    catch (...)
    {
        size = ptr->len;
        throw;
    }
}

bool CDataChannel::full_write_tofile(int file_fd, size_t& size, size_t offset)
{
    
    char* buffer = new char[sys::CUtil::get_page_size()];
    util::DeleteHelper<char> dh(buffer, true);    
    size_t remaining_size = size;
    size_t current_offset = offset;
     
    for (;;)
    {
        ssize_t retval = CDataChannel::receive(buffer, remaining_size);
        if (0 == retval) 
        {
            // 连接被对端关闭  
            size = size - remaining_size;
            throw sys::CSyscallException(-1, __FILE__, __LINE__);
        }
        else if (-1 == retval)
        {
            // 连接异常
            size = size - remaining_size;
            return false;
        }
        else
        {
            int written = pwrite(file_fd, buffer, retval, current_offset);
            if (written != retval)
            {
                size = size - remaining_size;
                throw sys::CSyscallException((-1 == written)? errno: EIO, __FILE__, __LINE__); 
            }

            current_offset += written;
            remaining_size -= written;

            // 全部接收完成
            if (0 == remaining_size) break;
        }
    }

    size = size - remaining_size;
    return true;
}

ssize_t CDataChannel::readv(const struct iovec *iov, int iovcnt)
{
    ssize_t retval;
    
    for (;;)
    {
        retval = ::readv(_fd, iov, iovcnt);

        if (retval != -1) break;      
        if (EINTR == errno) continue;
        if (EWOULDBLOCK == errno) break;

        throw sys::CSyscallException(errno, __FILE__, __LINE__);
    }

    atomic_add(retval, &gs_recv_buffer_bytes);
    return retval;
}

ssize_t CDataChannel::writev(const struct iovec *iov, int iovcnt)
{
    ssize_t retval;
    
    for (;;)
    {
        retval = ::writev(_fd, iov, iovcnt);

        if (retval != -1) break;      
        if (EINTR == errno) continue;
        if (EWOULDBLOCK == errno) break;

        throw sys::CSyscallException(errno, __FILE__, __LINE__);
    }

    atomic_add(retval, &gs_send_buffer_bytes);
    return retval;
}

NET_NAMESPACE_END
