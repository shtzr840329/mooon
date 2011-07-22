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
#ifndef MOOON_DISPATCHER_LOG_H
#define MOOON_DISPATCHER_LOG_H
#include "dispatcher/dispatcher.h"
MOOON_NAMESPACE_BEGIN
    
#define DISPATCHER_LOG_BIN(log, size)         __MYLOG_BIN(dispatcher::logger, log, size)
#define DISPATCHER_LOG_TRACE(format, ...)     __MYLOG_TRACE(dispatcher::logger, format, ##__VA_ARGS__)
#define DISPATCHER_LOG_FATAL(format, ...)     __MYLOG_FATAL(dispatcher::logger, format, ##__VA_ARGS__)
#define DISPATCHER_LOG_ERROR(format, ...)     __MYLOG_ERROR(dispatcher::logger, format, ##__VA_ARGS__)
#define DISPATCHER_LOG_WARN(format, ...)      __MYLOG_WARN(dispatcher::logger, format, ##__VA_ARGS__)
#define DISPATCHER_LOG_INFO(format, ...)      __MYLOG_INFO(dispatcher::logger, format, ##__VA_ARGS__)
#define DISPATCHER_LOG_DEBUG(format, ...)     __MYLOG_DEBUG(dispatcher::logger, format, ##__VA_ARGS__)

namespace dispatcher {

/***
  * �ַ���Ϣ����
  */
typedef enum
{
    DISPATCH_FILE,   /** ��Ҫ���͵���һ���ļ� */
    DISPATCH_BUFFER, /** ��Ҫ���͵���һ��Buffer */
    DISPATCH_STOP    /** ֹͣSender��Ϣ */
}dispatch_type_t;

/***
  * �ַ���Ϣͷ
  */
typedef struct
{
    dispatch_type_t type; /** �ַ���Ϣ���� */
    size_t length;        /** �ļ���С��content���ֽ��� */
    char data[0];
}message_t;

extern message_t* create_message();
extern message_t* create_stop_message();
extern void destroy_message(message_t* message);

} // namespace dispatcher

MOOON_NAMESPACE_END
#endif // MOOON_DISPATCHER_LOG_H
