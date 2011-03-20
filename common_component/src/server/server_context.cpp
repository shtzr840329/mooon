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
#include <signal.h>
#include <sys/sys_util.h>
#include "server_context.h"
MOOON_NAMESPACE_BEGIN

// 模块日志器
sys::ILogger* g_server_logger = NULL;

//////////////////////////////////////////////////////////////////////////
// 导出函数
void destroy_server(IServer* server)
{
    delete (CServerContext*)server;
}

IServer* create_server(sys::ILogger* logger, IServerConfig* config, IServerFactory* factory)
{
    g_server_logger = logger;
    return new CServerContext(config, factory);    
}

//////////////////////////////////////////////////////////////////////////
// CServerContext

CServerContext::~CServerContext()
{
}

CServerContext::CServerContext(IServerConfig* config, IServerFactory* factory)
    :_config(config)
    ,_factory(factory)
{
}

void CServerContext::stop()
{
    _thread_pool.destroy();
    _listen_manager.destroy();
}

bool CServerContext::start()
{       
    try
    {
        // 忽略PIPE信号
        if (!IgnorePipeSignal()) return false;

        create_listen_manager();
        create_thread_pool(&_listen_manager);
        return true;
    }
    catch (sys::CSyscallException& ex)
    {
		SERVER_LOG_FATAL("Created context failed for %s at %s:%d.\n", strerror(ex.get_errcode()), ex.get_filename(), ex.get_linenumber());
        return false;
    }
}

bool CServerContext::IgnorePipeSignal()
{
    // 忽略PIPE信号
    if (SIG_ERR == signal(SIGPIPE, SIG_IGN))
    {
        SERVER_LOG_FATAL("Can not ignore PIPE signal for %s.\n", strerror(errno));
        return false;
    }
    else
    {
        SERVER_LOG_INFO("Ignore PIPE signal success.\n");
        return true;
    }
}

void CServerContext::create_listen_manager()
{
	SERVER_LOG_INFO("Started to create listen manager.\n");

    const net::ip_port_pair_array_t& listen_parameter = _config->get_listen_parameter();
    for (net::ip_port_pair_array_t::size_type i=0; i<listen_parameter.size(); ++i)
    {
        _listen_manager.add(listen_parameter[i].first, listen_parameter[i].second);
		SERVER_LOG_INFO("Added listener %s:%d.\n", listen_parameter[i].first.to_string().c_str(), listen_parameter[i].second);
    }

	_listen_manager.create();
	SERVER_LOG_INFO("Created listen manager success.\n");
}

void CServerContext::create_thread_pool(net::CListenManager<CServerListener>* listen_manager)
{
	SERVER_LOG_INFO("Started to create waiter thread pool.\n");

	// 创建线程池
	_thread_pool.create(_config->get_thread_number());	

	uint16_t thread_count = _thread_pool.get_thread_count();
	CServerThread** thread_array = _thread_pool.get_thread_array();

	// 设置线程运行时参数
	for (uint16_t i=0; i<thread_count; ++i)
	{
		uint16_t listen_count = listen_manager->get_listener_count();
		CServerListener* listener_array = listen_manager->get_listener_array();

		thread_array[i]->set_context(this);
		thread_array[i]->add_listener_array(listener_array, listen_count);		
		thread_array[i]->wakeup();
	}

	SERVER_LOG_INFO("Created waiter thread pool success.\n");
}

MOOON_NAMESPACE_END
