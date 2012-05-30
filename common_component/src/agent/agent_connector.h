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
#ifndef MOOON_AGENT_CONNECTOR_H
#define MOOON_AGENT_CONNECTOR_H
#include <net/tcp_client.h>
#include "recv_machine.h"
#include "send_machine.h"
AGENT_NAMESPACE_BEGIN

class CAgentThread;
class CAgentConnector: public net::CTcpClient
{
public:
    CAgentConnector(CAgentThread* thread);
        
private:
    virtual void before_close();
    virtual net::epoll_event_t handle_epoll_event(void* input_ptr, uint32_t events, void* ouput_ptr);
    
private:
    net::epoll_event_t handle_error(void* input_ptr, void* ouput_ptr);
    net::epoll_event_t handle_input(void* input_ptr, void* ouput_ptr);
    net::epoll_event_t handle_output(void* input_ptr, void* ouput_ptr);    
    
private:
    CAgentThread* _thread;    
    CRecvMachine _recv_machine;
    CSendMachine _send_machine;        
};

AGENT_NAMESPACE_END
#endif // MOOON_AGENT_CONNECTOR_H

