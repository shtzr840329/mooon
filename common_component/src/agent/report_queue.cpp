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
#include "report_queue.h"
#include "agent_thread.h"
MOOON_NAMESPACE_BEGIN

CReportQueue::CReportQueue(uint32_t queue_max)
    :net::CEpollableQueue<util::CArrayQueue<report_message_t*> >(queue_max)
{
}

net::epoll_event_t CReportQueue::handle_epoll_event(void* input_ptr, uint32_t events, void* ouput_ptr)
{    
    return net::epoll_write;
}

MOOON_NAMESPACE_END
