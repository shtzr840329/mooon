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
 * Author: JianYi, eyjian@qq.com or eyjian@gmail.com
 */
#include "observer_context.h"
MOOON_NAMESPACE_BEGIN

CObserverContext::CObserverContext(IDataReporter* data_reporter, uint16_t report_frequency_seconds)
	:_data_reporter(data_reporter)
	,_report_frequency_seconds(report_frequency_seconds)
{
	_observer_thread = new CObserverThread(this);
}

bool CObserverContext::create()
{
	try
	{
		_observer_thread->start();
	}
	catch (sys::CSyscallException& ex)
	{
		OBSERVER_LOG_FATAL("Created observer manager failed for %s at %s:%d.\n", strerror(ex.get_errcode()), ex.get_filename(), ex.get_linenumber());
		return false;
	}

	return true;
}

void CObserverContext::destroy()
{
	_observer_thread->stop();
	_observer_thread->dec_refcount();
}

void CObserverContext::register_observee(IObservable* observee)
{
    sys::LockHelper<sys::CLock> lock_helper(_lock);
	_observee_set.insert(observee);
}

void CObserverContext::deregister_objservee(IObservable* observee)
{
    sys::LockHelper<sys::CLock> lock_helper(_lock);
    _observee_set.erase(observee);
}

void CObserverContext::collect()
{
    sys::LockHelper<sys::CLock> lock_helper(_lock);
	for (std::set<IObservable*>::iterator iter=_observee_set.begin(); iter!=_observee_set.end(); ++iter)
	{
		IObservable* observee = *iter;
		observee->on_report(_data_reporter);
	}
}

//////////////////////////////////////////////////////////////////////////
// 全局函数
namespace observer
{
    sys::ILogger* logger = NULL;
}

static CObserverContext* g_observer_context = NULL;

void destroy_observer_manager()
{
    if (g_observer_context != NULL)
    {
        g_observer_context->destroy();
        delete g_observer_context;
        g_observer_context = NULL;
    }
}

IObserverManager* get_observer_manager()
{
    return g_observer_context;
}

IObserverManager* create_observer_manager(IDataReporter* data_reporter, uint16_t report_frequency_seconds)
{    
    if (NULL == g_observer_context) 
    {
        g_observer_context = new CObserverContext(data_reporter, report_frequency_seconds);
        if (!g_observer_context->create())
        {
            destroy_observer_manager();
        }
    }
    
    return g_observer_context;
}

MOOON_NAMESPACE_END
