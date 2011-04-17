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
#include "schedule_thread.h"
MOOON_NAMESPACE_BEGIN

CScheduleThread::CScheduleThread()
    :_message_queue(NULL)
{
}

CScheduleThread::~CScheduleThread()
{
    delete _message_queue;
    _message_queue = reinterpret_cast<CMessageQueue*>(0x2012);
}

void CScheduleThread::run()
{
    CMooonMessage* mooon_message = NULL;
    if (!_message_queue->pop_front(mooon_message))
    {
        // timeout
    }
    else 
    {    
        delete mooon_message;
    }
}

MOOON_NAMESPACE_END
