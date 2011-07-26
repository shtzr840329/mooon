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
#ifndef MOOON_AGENT_CENTER_CONNECTOR_H
#define MOOON_AGENT_CENTER_CONNECTOR_H
#include <sys/log.h>
#include <net/tcp_client.h>
#include "report_queue.h"
#include "agent/agent_message.h"
AGENT_NAMESPACE_BEGIN

class CAgentContext;
class CCenterConnector: public net::CTcpClient
{
public:
    ~CCenterConnector();
    CCenterConnector(CReportQueue* report_queue);
        
    void send_heartbeat();

private:
    virtual net::epoll_event_t handle_epoll_event(void* input_ptr, uint32_t events, void* ouput_ptr);

private:
    net::epoll_event_t handle_epoll_read(void* input_ptr, void* output_ptr);
    net::epoll_event_t handle_epoll_write(void* input_ptr, void* output_ptr);
    net::epoll_event_t handle_epoll_error(void* input_ptr, void* output_ptr);

private:
    CReportQueue* _report_queue;
};

AGENT_NAMESPACE_END
#endif // MOOON_AGENT_CENTER_CONNECTOR_H
