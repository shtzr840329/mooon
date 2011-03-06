﻿/**
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
#include "sender.h"
#include "send_queue.h"
#include "send_thread.h"
MOOON_NAMESPACE_BEGIN

CSendQueue::CSendQueue(uint32_t queue_max, CSender* sender)
    :net::CEpollableQueue<util::CArrayQueue<dispatch_message_t*> >(queue_max)
    ,_sender(sender)
{
}

net::epoll_event_t CSendQueue::handle_epoll_event(void* ptr, uint32_t events)
{
    // 通知CSender去发送消息
    CSendThread* thread = (CSendThread*)ptr;
    net::CEpoller& epoller = thread->get_epoller();

    epoller.set_events(_sender, EPOLLOUT);
    return net::epoll_remove;
}

MOOON_NAMESPACE_END
