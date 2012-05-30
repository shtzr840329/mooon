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
#include "processor_manager.h"
AGENT_NAMESPACE_BEGIN
    
bool CProcessorManager::register_processor(ICommandProcessor* processor)
{
    std::pair<CommandProcessorMap::iterator, bool> ret;
    sys::LockHelper<sys::CLock> lh(_lock);
    ret = _processor_map.insert(std::make_pair(processor->get_command(), processor));
    
    return ret.second;
}

void CProcessorManager::deregister_processor(ICommandProcessor* processor)
{
    sys::LockHelper<sys::CLock> lh(_lock);
    _processor_map.erase(processor->get_command());    
}

bool CProcessorManager::on_message(const agent_message_header_t& header, size_t finished_size, const char* buffer, size_t buffer_size)
{
    // ������������Ǻܺõ���������Ϊprocessor->on_message()
    // ����Ϊ�ǲ��ɿصģ��ϼѵ������ǽ�ICommandProcessor��ɿ����ü����ģ�
    // ���������Ӧ�ó���������ȡ����Ϊregister_processor()��
    // deregister_process()Ӧ�����ٱ���̬���ã�һ�����ڳ�ʼ��ʱ����
    sys::LockHelper<sys::CLock> lh(_lock);
    CommandProcessorMap::iterator iter = _processor_map.find(header.command);
    if (iter != _processor_map.end())
    {
        return false;
    }
            
    ICommandProcessor* processor = iter->second;
    TMessageContext msg_ctx(header.size, finished_size);    
    return processor->on_message(msg_ctx, buffer, buffer_size);
}

AGENT_NAMESPACE_END
