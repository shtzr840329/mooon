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
#ifndef MOOON_AGENT_THREAD_H
#define MOOON_AGENT_THREAD_H
#include <sys/thread.h>
#include <net/epoller.h>
#include "agent_log.h"
#include "report_queue.h"
#include "center_connector.h"
MOOON_NAMESPACE_BEGIN

class CAgentContext;
class CAgentThread: public sys::CThread
{
public:
    CAgentThread(CAgentContext* context, uint32_t queue_max);
    ~CAgentThread();
    
    void report(const char* data, uint16_t data_size);
    void add_center(const net::ip_address_t& ip_address);
    void process_command(const agent_message_header_t* header, char* body, uint32_t body_size);

    IConfigObserver* get_config_observer(const char* config_name);
    void deregister_config_observer(const char* config_name);
    bool register_config_observer(const char* config_name, IConfigObserver* config_observer);
    
private:
    virtual void run();
    virtual bool before_start();

private:
    void reset_center();
    void connect_center();
    void close_connector();
    bool choose_center(uint32_t& center_ip, uint16_t& center_port);

private:    
    net::CEpoller _epoller;
    CAgentContext* _context;
    CReportQueue _report_queue;
    CCenterConnector _center_connector;
    std::map<uint32_t, uint16_t> _valid_center;   /** 存储有效的Center, 初始化时均为有效，当有连接失败时就切到无效容器中 */
    std::map<uint32_t, uint16_t> _invalid_center; /** 存储无效的Center */
    std::map<std::string, IConfigObserver*> _config_observer_map;
};

MOOON_NAMESPACE_END
#endif // MOOON_AGENT_THREAD_H
