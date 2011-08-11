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
#ifndef MOOON_ECHO_SERVER_PACKET_HANDLER_IMPL_H
#define MOOON_ECHO_SERVER_PACKET_HANDLER_IMPL_H
#include <server/server.h>

class CPakcetHandlerImpl: public server::IPacketHandler
{
public:    
    CPakcetHandlerImpl(server::IConnection* connection);
    ~CPakcetHandlerImpl();

private:  
    virtual void reset();

    virtual char* get_request_buffer();
    virtual size_t get_request_size() const;        
    virtual util::handle_result_t on_handle_request(size_t data_size, server::Indicator& indicator);
             
    virtual const char* get_response_buffer() const;
    virtual size_t get_response_size() const;
    virtual size_t get_response_offset() const;
    virtual void move_response_offset(size_t offset);
    virtual util::handle_result_t on_response_completed(server::Indicator& indicator);

private:
    server::IConnection* _connection; // ����������
    size_t _request_size;    // �������󻺳���������ֽ���
    size_t _response_size;   // ��Ҫ���͵���Ӧ�����ֽ���
    size_t _response_offset; // ��ǰ�Ѿ����͵���Ӧ�����ֽ���
    char* _request_buffer;   // ��������������ݵĻ�����
};

#endif // MOOON_ECHO_SERVER_PACKET_HANDLER_IMPL_H
