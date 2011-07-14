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
#include <sys/close_helper.h>
#include <util/string_util.h>
#include "default_reply_handler.h"
#include "unmanaged_sender_table.h"
MOOON_NAMESPACE_BEGIN
namespace dispatcher {

CUnmanagedSenderTable::CUnmanagedSenderTable(uint32_t queue_max, CSendThreadPool* thread_pool)
    :CSenderTable(queue_max, thread_pool)
{
}

void CUnmanagedSenderTable::close_sender(IUnmanagedSender* sender)
{
    CUnmanagedSender* sender_impl = (CUnmanagedSender*)sender;
   
    uint16_t port = sender_impl->get_peer_port();
    const uint32_t* ip_data = sender_impl->get_peer_ip().get_address_data();
                
    if (sender_impl->is_ipv6())
    {
        // IPV6
        net::ipv6_node_t ipv6_node(port, ip_data);  
        close_sender(ipv6_node);        
    }
    else
    {
        // IPV4
        net::ipv4_node_t ipv4_node(port, ip_data[0]);  
        close_sender(ipv4_node);        
    }    
}

void CUnmanagedSenderTable::close_sender(const net::ipv4_node_t& ip_node)
{
    sys::LockHelper<sys::CLock> lock_helper(_ipv4_lock);
    do_close_sender<net::ipv4_hash_map<CUnmanagedSender*>, net::ipv4_node_t>(_ipv4_sender_table, ip_node);
}

void CUnmanagedSenderTable::close_sender(const net::ipv6_node_t& ip_node)
{
    sys::LockHelper<sys::CLock> lock_helper(_ipv6_lock);
    do_close_sender<net::ipv6_hash_map<CUnmanagedSender*>, net::ipv6_node_t>(_ipv6_sender_table, ip_node);
}

CUnmanagedSender* CUnmanagedSenderTable::open_sender(const net::ipv4_node_t& ip_node)
{
    sys::LockHelper<sys::CLock> lock_helper(_ipv4_lock);
    return open_sender<net::ipv4_hash_map<CUnmanagedSender*>, net::ipv4_node_t>(_ipv4_sender_table, ip_node);
}

CUnmanagedSender* CUnmanagedSenderTable::open_sender(const net::ipv6_node_t& ip_node)
{
    sys::LockHelper<sys::CLock> lock_helper(_ipv6_lock);
    return open_sender<net::ipv6_hash_map<CUnmanagedSender*>, net::ipv6_node_t>(_ipv6_sender_table, ip_node);
}

void CUnmanagedSenderTable::set_resend_times(const net::ipv4_node_t& ip_node, int8_t resend_times)
{
    do_set_resend_times<net::ipv4_node_t>(ip_node, resend_times);
}

void CUnmanagedSenderTable::set_resend_times(const net::ipv6_node_t& ip_node, int8_t resend_times)
{
    do_set_resend_times<net::ipv6_node_t>(ip_node, resend_times);
}

bool CUnmanagedSenderTable::send_message(const net::ipv4_node_t& ip_node, message_t* message, uint32_t milliseconds)
{
    return do_send_message<net::ipv4_node_t>(ip_node, message, milliseconds);
}

bool CUnmanagedSenderTable::send_message(const net::ipv6_node_t& ip_node, message_t* message, uint32_t milliseconds)
{
    return do_send_message<net::ipv6_node_t>(ip_node, message, milliseconds);
}

template <typename ip_node_t>
CUnmanagedSender* CUnmanagedSenderTable::new_sender(const ip_node_t& ip_node)
{
    IReplyHandler* reply_handler = NULL;
    CSendThreadPool* thread_pool = get_thread_pool();
    IFactory* reply_handler_factory = thread_pool->get_reply_handler_factory();
    if (NULL == reply_handler_factory)
    {
        reply_handler = new CDefaultReplyHandler;
    }
    else
    {
        reply_handler = reply_handler_factory->create_reply_handler();
    }

    CUnmanagedSender* sender = new CUnmanagedSender(thread_pool, -1, get_queue_max(), reply_handler);
    sender->inc_refcount(); // 由close_sender来减
    sender->set_peer(ip_node);

    CSendThread* thread = thread_pool->get_next_thread();
    sender->inc_refcount();
    thread->add_sender(sender);

    return sender;
}

template <typename ip_node_t>
void CUnmanagedSenderTable::do_set_resend_times(const ip_node_t& ip_node, int8_t resend_times)
{
    CUnmanagedSender* sender = open_sender(ip_node);
    if (sender != NULL)
    {
        sender->set_resend_times(resend_times);
        close_sender(sender);
    }    
}

template <typename ip_node_t>
bool CUnmanagedSenderTable::do_send_message(const ip_node_t& ip_node, message_t* message, uint32_t milliseconds)
{
    CUnmanagedSender* sender = open_sender(ip_node);
    if (NULL == sender) return false;
    
    bool retval = sender->push_message(message, milliseconds);
    close_sender(sender);

    return retval;
}

template <class SenderTableType, class IpNodeType>
CUnmanagedSender* CUnmanagedSenderTable::open_sender(SenderTableType& sender_table, const IpNodeType& ip_node)
{
    CUnmanagedSender* sender = NULL;
    typename SenderTableType::iterator iter = sender_table.find(const_cast<IpNodeType*>(&ip_node));
    if (iter == sender_table.end())
    {
        // 新建立一个
        sender = new_sender(ip_node); 
        
        IpNodeType* new_ip_node = new IpNodeType(ip_node);        
        sender_table.insert(std::make_pair(new_ip_node, sender));
    }
    else
    {
        sender = iter->second;
    }
    if (sender != NULL)
    {
        sender->inc_refcount();
    }

    return sender;
}

template <class SenderTableType, class IpNodeType>
void CUnmanagedSenderTable::do_close_sender(SenderTableType& sender_table, const IpNodeType& ip_node)
{
    typename SenderTableType::iterator iter = sender_table.find(const_cast<IpNodeType*>(&ip_node));    

    // 如果没有找到，则直接返回
    if (iter != sender_table.end())    
    {                
        if (iter->second->dec_refcount()) // 减引用计数，这个时候SendThread可能还在用它
        {
            delete iter->first;
            sender_table.erase(iter);
        }
    }
}

} // namespace dispatcher
MOOON_NAMESPACE_END
