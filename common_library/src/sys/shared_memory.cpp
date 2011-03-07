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
 * Author: eyjian@qq.com or eyjian@gmail.com
 */
#include "sys/shared_memory.h"
SYS_NAMESPACE_BEGIN

CSysVSharedMemory::CSysVSharedMemory()
    :_shmid(-1)
    ,_shmaddr(NULL)
{
}

void CSysVSharedMemory::open(const char* path)
{
    if (NULL == path)
        throw CSyscallException(EINVAL, __FILE__, __LINE__, "path null");

    key_t key = ftok(path, getpid());
    if (-1 == key)
        throw CSyscallException(errno, __FILE__, __LINE__, "ftok error");

    _shmid = shmget(key, 1, 0);
    if (-1 == _shmid)    
        throw CSyscallException(errno, __FILE__, __LINE__, "shmget error");    
}

bool CSysVSharedMemory::create(const char* path, mode_t mode)
{
    key_t key = IPC_PRIVATE;
    
    // 得到IPC键
    if (path != NULL)
    {    
        key_t key = ftok(path, getpid());
        if (-1 == key)
            throw CSyscallException(errno, __FILE__, __LINE__, "ftok error");
    }

    // 创建共享内存
    _shmid = shmget(key, 1, IPC_CREAT | IPC_EXCL | mode);
    if (-1 == _shmid)
    {
        if (EEXIST == errno) return false;
        throw CSyscallException(errno, __FILE__, __LINE__, "shmget error");
    }
        
    return true;
}

void CSysVSharedMemory::close()
{
    if (_shmid != -1)
    {    
        //struct shmid_ds buf;
        if (-1 == shmctl(_shmid, IPC_RMID, NULL))
            throw CSyscallException(errno, __FILE__, __LINE__, "remove shared memory error");

        _shmid = -1;
    }
}

void CSysVSharedMemory::detach()
{
    if (_shmaddr != NULL)
    {
        shmdt(_shmaddr);
        _shmaddr = NULL;
    }
}

void* CSysVSharedMemory::attach(int flag)
{
    if (_shmaddr != NULL)
    {    
        _shmaddr = shmat(_shmid, NULL, flag);
        if ((void *)-1 == _shmaddr)
            throw CSyscallException(errno, __FILE__, __LINE__, "shmat error");
    }

    return _shmaddr;
}

void* CSysVSharedMemory::get_shared_memoty_address()
{
    return _shmaddr;
}

void* CSysVSharedMemory::get_shared_memoty_address() const
{
    return _shmaddr;
}

SYS_NAMESPACE_END
