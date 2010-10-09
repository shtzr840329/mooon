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
#ifndef _UNMANAGED_SENDER_TABLE_H
#define _UNMANAGED_SENDER_TABLE_H
#include <net/ip_node.h>
#include "sender_table.h"
#include "unmanaged_sender.h"
MY_NAMESPACE_BEGIN

class CUnmanagedSenderTable: public CSenderTable
{
public:
    CUnmanagedSenderTable(uint32_t queue_max, CSendThreadPool* thread_pool);
    
    void release_sender(ISender* sender);

    void close_sender(const net::ipv4_node_t& ip_node);
    void close_sender(const net::ipv6_node_t& ip_node);

    CUnmanagedSender* get_sender(const net::ipv4_node_t& ip_node);
    CUnmanagedSender* get_sender(const net::ipv6_node_t& ip_node);        

    void set_object(const net::ipv4_node_t& ip_node, void* object);
    void set_object(const net::ipv6_node_t& ip_node, void* object);

    bool send_message(const net::ipv4_node_t& ip_node, dispach_message_t* message);
    bool send_message(const net::ipv6_node_t& ip_node, dispach_message_t* message);
    
private:        
    template <typename ip_node_t>
    CUnmanagedSender* new_sender(const ip_node_t& ip_node);    
    
    template <typename ip_node_t>
    bool do_send_message(const ip_node_t& ip_node, dispach_message_t* message);

    template <class SenderTableType, class IpNodeType>
    CUnmanagedSender* get_sender(SenderTableType& sender_table, const IpNodeType& ip_node);
    
    template <class SenderTableType, class IpNodeType>
    void do_close_sender(SenderTableType& sender_table, const IpNodeType& ip_node);

    template <class SenderTableType, class IpNodeType>
    void do_release_sender(SenderTableType& sender_table, const IpNodeType& ip_node);    
    
private:
    sys::CLock _ipv4_lock;
    sys::CLock _ipv6_lock;
    net::ipv4_hash_map<CUnmanagedSender*> _ipv4_sender_table;
    net::ipv6_hash_map<CUnmanagedSender*> _ipv6_sender_table;
};

MY_NAMESPACE_END
#endif // _UNMANAGED_SENDER_TABLE_H
