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
 * Author: jian yi, eyjian@qq.com
 */
#include <net/net_util.h>
#include "server_thread.h"
#include "server_context.h"
MOOON_NAMESPACE_BEGIN

CServerThread::CServerThread()
    :_waiter_pool(NULL)
    ,_packet_handler(NULL)
    ,_context(NULL)
{
    _current_time = time(NULL);
    _timeout_manager.set_timeout_handler(this);
    
    _pending_waiter_queue = new util::CArrayQueue<CWaiter*>(100);
}

CServerThread::~CServerThread()
{
	_epoller.destroy();
    delete _pending_waiter_queue;
}

void CServerThread::run()
{
    int retval;

    try
    {        
        _timeout_manager.check_timeout(_current_time);
        check_pending_queue();

        // EPOLL检测
        retval = _epoller.timed_wait(_context->get_config()->get_epoll_timeout_milliseconds());        
    }
    catch (sys::CSyscallException& ex)
    {
		SERVER_LOG_FATAL("Waiter thread wait error for %s.\n", ex.to_string().c_str());
        throw; // timed_wait异常是不能恢复的
    }

	try
    {		
        // 得到当前时间
        _current_time = time(NULL);

        if (0 == retval) // timeout
        {
            _packet_handler->timeout(_current_time);
            return;
        }
	
		for (int i=0; i<retval; ++i)
		{
            uint16_t takeover_thread_index = 0;
			net::CEpollable* epollable = _epoller.get(i);
			net::epoll_event_t retval = epollable->handle_epoll_event(this, _epoller.get_events(i), &takeover_thread_index);

            // 处理结果
            switch (retval)
            {
            case net::epoll_read:
                // 切换到只收数据状态
                _epoller.set_events(epollable, EPOLLIN);
                break;
            case net::epoll_write:
                // 切换到只发数据状态
                _epoller.set_events(epollable, EPOLLOUT);
                break;
            case net::epoll_read_write:
                // 切换到收发数据状态
                _epoller.set_events(epollable, EPOLLIN|EPOLLOUT);
                break;
            case net::epoll_close:
                // 关闭连接
                del_waiter((CWaiter*)epollable);
                break;
            case net::epoll_release:
                // 从epoll中移除连接，但不关闭连接
                remove_waiter((CWaiter*)epollable);
                handover_waiter((CWaiter*)epollable, takeover_thread_index);
                break;
            default: // net::epoll_none
                // nothing to do
                break;
            }
		}
	}
    catch (sys::CSyscallException& ex)
    {
		SERVER_LOG_FATAL("Waiter thread run error for %s.\n", ex.to_string().c_str());
    }
}

bool CServerThread::before_start()
{
    try
    {
        _packet_handler = _context->get_factory()->create_packet_handler();
        _timeout_manager.set_timeout_seconds(_context->get_config()->get_connection_timeout_seconds());       
        _epoller.create(_context->get_config()->get_epoll_size());

        // 建立连接池
        uint32_t thread_connection_pool_size = _context->get_config()->get_connection_pool_size()
            / _context->get_config()->get_thread_number();

        _waiter_pool = new CWaiterPool(_context->get_factory(), thread_connection_pool_size);        
        return true;
    }
    catch (sys::CSyscallException& ex)
    {
        return false;
    }
}

void CServerThread::set_parameter(void* parameter)
{
    _context = static_cast<CServerContext*>(parameter);
}

void CServerThread::on_timeout_event(CWaiter* waiter)
{	
    _epoller.del_events(waiter);
    _waiter_pool->push_waiter(waiter);
}

uint16_t CServerThread::index() const
{
    return get_index();
}

bool CServerThread::takeover_waiter(CWaiter* waiter)
{
    if (_pending_waiter_queue->is_full()) return false;
    
    sys::CLockHelper<sys::CLock> lock_helper(_pending_lock);
    if (_pending_waiter_queue->is_full()) return false;
    
    _pending_waiter_queue->push_back(waiter);
    return true;
}

void CServerThread::check_pending_queue()
{
    if (!_pending_waiter_queue->is_empty())
    {
        sys::CLockHelper<sys::CLock> lock_helper(_pending_lock);
        while (!_pending_waiter_queue->is_empty())
        {
            CWaiter* waiter = _pending_waiter_queue->pop_front();
            watch_waiter(waiter);
        }
    }
}

bool CServerThread::watch_waiter(CWaiter* waiter)
{
    try
    {               
        _epoller.set_events(waiter, EPOLLIN);
        _timeout_manager.push(waiter, _current_time);

        return true;
    }
    catch (sys::CSyscallException& ex)
    {
        _waiter_pool->push_waiter(waiter);
        SERVER_LOG_ERROR("Set %s epoll events error: %s.\n"
            , waiter->to_string().c_str()
            , ex.to_string().c_str());
        
        return false;
    }
}

void CServerThread::handover_waiter(CWaiter* waiter, uint16_t takeover_thread_index)
{
    CServerThread* takeover_thread = _context->get_thread(takeover_thread_index);
    if (NULL == takeover_thread)
    {
        SERVER_LOG_ERROR("Takeover thread %u not exist for %s.\n", takeover_thread_index, waiter->to_string().c_str());
        _waiter_pool->push_waiter(waiter);
    }
    else if (takeover_thread->takeover_waiter(waiter))
    {
        SERVER_LOG_DEBUG("Handover %s from %u to %u.\n", waiter->to_string().c_str(), get_index(), takeover_thread_index);
    }
    else
    {
        SERVER_LOG_ERROR("Can not handover %s from %u to %u.\n", waiter->to_string().c_str(), get_index(), takeover_thread_index);
        _waiter_pool->push_waiter(waiter);
    }
}

void CServerThread::del_waiter(CWaiter* waiter)
{    
    remove_waiter(waiter);
    _waiter_pool->push_waiter(waiter);
}

void CServerThread::remove_waiter(CWaiter* waiter)
{
    try
    {
        _epoller.del_events(waiter);		
        _timeout_manager.remove(waiter);        
    }
    catch (sys::CSyscallException& ex)
    {
        SERVER_LOG_ERROR("Delete waiter error for %s.\n", ex.to_string().c_str());
    }    
}

void CServerThread::update_waiter(CWaiter* waiter)
{
    _timeout_manager.remove(waiter);
    _timeout_manager.push(waiter, _current_time);
}

bool CServerThread::add_waiter(int fd, const net::ip_address_t& peer_ip, net::port_t peer_port)
{
    CWaiter* waiter = _waiter_pool->pop_waiter();
    if (NULL == waiter)
    {
        SERVER_LOG_WARN("Waiter overflow - %s:%d.\n", peer_ip.to_string().c_str(), peer_port);
        return false;
    }    
    
    waiter->attach(fd, peer_ip, peer_port);
    return watch_waiter(waiter);    
}

void CServerThread::add_listener_array(CServerListener* listener_array, uint16_t listen_count)
{	    
    for (uint16_t i=0; i<listen_count; ++i)
         _epoller.set_events(&listener_array[i], EPOLLIN, true);    
}

MOOON_NAMESPACE_END
