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
#ifndef MOOON_RECEIVER_CONFIG_H
#define MOOON_RECEIVER_CONFIG_H
#include <server/server.h>
MOOON_NAMESPACE_BEGIN

class CReceiverConfig: public IServerConfig
{
private:
    /** �õ�epoll��С */
    virtual uint32_t get_epoll_size() const;

    /** �õ�epool�ȴ���ʱ������ */
    virtual uint32_t get_epoll_timeout() const;

    /** �õ���ܵĹ����̸߳��� */
    virtual uint16_t get_thread_number() const;

    /** �õ����ӳش�С */
    virtual uint32_t get_connection_pool_size() const;

    /** ���ӳ�ʱ���� */
    virtual uint32_t get_connection_timeout_seconds() const;

    /** �õ��������� */    
    virtual const net::ip_port_pair_array_t& get_listen_parameter() const;
};

MOOON_NAMESPACE_END
#endif // MOOON_RECEIVER_CONFIG_H
