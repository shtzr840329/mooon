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
 * Author: eyjian@gmail.com, eyjian@qq.com
 *
 */
#ifndef MOOON_SCHEDULER_SERVICE_H
#define MOOON_SCHEDULER_SERVICE_H
#include <sstream>
#include <string.h>
MOOON_NAMESPACE_BEGIN

/***
  * Service����ģʽ
  */
typedef enum
{
    SERVICE_RUN_MODE_THREAD, /** �߳�ģʽ */
    SERVICE_RUN_MODE_PROCESS /** ����ģʽ */
}service_run_mode_t;

/***
  * Service��Ϣ�ṹ��
  * ����Service�����ʱ��Ҫ�õ�
  */
typedef struct
{
    uint16_t id;                 /** Service ID */
    uint32_t version;            /** Servier�汾�� */    
    uint8_t thread_number;       /** ��ռ���̸߳��� */ 
    bool auto_activate_onload;   /** ����ʱ�Զ����� */
    service_run_mode_t run_mode; /** ����ģʽ */
    std::string name;            /** ���ƣ�Ҫ���Ӧ�Ĺ������Ϊ��lib$name.so */

    std::string to_string() const
    {
        std::stringstream ss;
        ss << "service://"<< id << ":" << version << ":" << name;
        return ss.str();
    }
}service_info_t;

/***
  * service_info_t�ĸ�ֵ����
  */
inline service_info_t& operator =(service_info_t& self, const service_info_t& other)
{
    memcpy(&self, &other, sizeof(service_info_t));
    return self;
}

/***
  * Service�ӿڶ���
  */
class IService
{
public:
    virtual bool on_load() = 0;
    virtual bool on_unload() = 0;

    virtual bool on_activate() = 0;
    virtual bool on_deactivate() = 0;

    virtual void on_create_session(bool is_little_endian, mooon_message_t* mooon_message) = 0;
    virtual void on_destroy_session(bool is_little_endian, mooon_message_t* mooon_message) = 0;

    virtual void on_request(bool is_little_endian, mooon_message_t* mooon_message) = 0;
    virtual void on_response(bool is_little_endian, mooon_message_t* mooon_message) = 0;    
};

MOOON_NAMESPACE_END
#endif // MOOON_SCHEDULER_SERVICE_H
