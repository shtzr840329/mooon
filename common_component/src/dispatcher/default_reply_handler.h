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
#ifndef DEFAULT_REPLY_HANDLER_H
#define DEFAULT_REPLY_HANDLER_H
#include "dispatcher/dispatcher.h"
MY_NAMESPACE_BEGIN

class CDefaultReplyHandler: public IReplyHandler
{
public:
    CDefaultReplyHandler();

private:  
    /** 得到存储应答消息的buffer */
    virtual char* get_buffer() const;

    /** 得到存储应答消息的buffer大小 */
    virtual uint32_t get_buffer_length() const;    

    /** 处理应答消息 */
    virtual bool handle_reply(void* object, int32_t node_id, const net::ip_address_t& peer_ip, uint16_t peer_port, uint32_t data_size);

private:
    mutable char _buffer[LINE_MAX];
};

MY_NAMESPACE_END
#endif // DEFAULT_REPLY_HANDLER_H
