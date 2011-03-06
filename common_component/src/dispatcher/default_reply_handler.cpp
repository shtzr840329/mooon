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
#include "default_reply_handler.h"
MOOON_NAMESPACE_BEGIN

CDefaultReplyHandler::CDefaultReplyHandler()
{
    _buffer[0] = '\0';
}

char* CDefaultReplyHandler::get_buffer()
{
    return _buffer;
}

uint32_t CDefaultReplyHandler::get_buffer_length() const
{
    return sizeof(_buffer);
}

void CDefaultReplyHandler::before_send(int32_t route_id, const net::ip_address_t& peer_ip, uint16_t peer_port)
{
    // do nothing
}

void CDefaultReplyHandler::send_finish(int32_t route_id, const net::ip_address_t& peer_ip, uint16_t peer_port)
{    
    // do nothing
}

void CDefaultReplyHandler::sender_closed(int32_t route_id, const net::ip_address_t& peer_ip, uint16_t peer_port)
{    
    // do nothing
}

void CDefaultReplyHandler::sender_connected(int32_t route_id, const net::ip_address_t& peer_ip, uint16_t peer_port)
{
    // do nothing
}

void CDefaultReplyHandler::sender_connect_failure(int32_t route_id, const net::ip_address_t& peer_ip, uint16_t peer_port)
{    
    // do nothing
}

util::handle_result_t CDefaultReplyHandler::handle_reply(int32_t route_id, const net::ip_address_t& peer_ip, uint16_t peer_port, uint32_t data_size)
{
    return util::handle_continue;
}

MOOON_NAMESPACE_END
