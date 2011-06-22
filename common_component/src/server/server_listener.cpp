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
#include <sys/thread.h>
#include <net/net_util.h>
#include "server_thread.h"
#include "server_listener.h"
MOOON_NAMESPACE_BEGIN

net::epoll_event_t CServerListener::handle_epoll_event(void* input_ptr, uint32_t events, void* ouput_ptr)
{           
    try
    {
        net::port_t peer_port;
        net::ip_address_t peer_ip;
        
        int newfd = accept(peer_ip, peer_port);
        
        CServerThread* thread = static_cast<CServerThread *>(input_ptr);
        if (!thread->add_waiter(newfd, peer_ip, peer_port))
        {
            net::close_fd(newfd);
        }
    }
    catch (sys::CSyscallException& ex)
    {
		// 对于某些server，这类信息巨大，如webserver
        SERVER_LOG_ERROR("Accept error: %s.\n", ex.to_string().c_str());            
    }
    
    return net::epoll_none;
}

MOOON_NAMESPACE_END
