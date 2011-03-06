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
 * Author: jian yi, eyjian@qq.com
 */
#ifndef SERVER_LISTENER_H
#define SERVER_LISTENER_H
#include <sys/log.h>
#include <net/listener.h>
#include "server_log.h"
MOOON_NAMESPACE_BEGIN

class CServerListener: public net::CListener
{
private:
    virtual net::epoll_event_t handle_epoll_event(void* ptr, uint32_t events);
};

MOOON_NAMESPACE_END
#endif // SERVER_LISTENER_H
