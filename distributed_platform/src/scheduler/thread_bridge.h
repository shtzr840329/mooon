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
 * Author: eyjian@gmail.com, eyjian@qq.com
 *
 */
#ifndef MOOON_SCHEDULER_THREAD_BRIDGE_H
#define MOOON_SCHEDULER_THREAD_BRIDGE_H
#include "service_bridge.h"
#include "message_handler.h"
MOOON_NAMESPACE_BEGIN

/***
  * 线程模式的桥
  */
class CThreadBridge: public IServiceBridge
{
public:
    CThreadBridge(IService* service);

private: // Implement IServiceBridge
    virtual void schedule(schedule_message_t* schedule_message);

private:
    CMessageHandler _message_handler;
};

MOOON_NAMESPACE_END
#endif // MOOON_SCHEDULER_THREAD_BRIDGE_H
