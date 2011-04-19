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
 * ���ȡ�ͨѶ��Ϣ�ṹ����
 */
#ifndef MOOON_SCHEDULER_MESSAGE_H
#define MOOON_SCHEDULER_MESSAGE_H
#include <net/net_util.h>
MOOON_NAMESPACE_BEGIN
#pragma pack(4)

/***
  * ��������
  */
enum
{
    /***
      * �ֽ�������
      * �����ֽ���Ϊ���ֽ���
      * �����ֽ�����CPU�й�
      */
    BYTE_ORDER_BIG_ENDIAN    = 0,
    BYTE_ORDER_LITTLE_ENDIAN = 1,
       
    /***
      * IP����
      */
    IP_TYPE_ID = 0, /** IPΪһ��IDֵ */
    IP_TYPE_V4 = 1, /** IPV4��ַ */
    IP_TYPE_V6 = 2, /** IPV6��ַ */

    /***
      * Service��Session��IDȡֵ��Χ
      */
    DEFAULT_MAX_SERVICE_ID = 100,   /** Ĭ�ϵ����Service IDֵ */
    DEFAULT_MAX_SESSION_ID = 100000 /** Ĭ�ϵ����Session IDֵ */
};

/***
  * mooon��Ϣ���ͣ�
  * ȡֵ��Χֻ��Ϊ0~127����0x00~0x7F
  */
typedef enum
{
    /***
      * mooon��Ϣȡֵ��Χ
      */
    MOOON_MESSAGE_MIN = 0x00,   /** mooon��Ϣ���͵���Сֵ */
    MOOON_MESSAGE_MAX = 0x7F,   /** mooon��Ϣ���͵����ֵ */

    /***
      * Session��Ϣȡֵ��Χ
      */
    SESSION_MESSAGE_MIN = 0x1F,   /** Session��Ϣ���͵���Сֵ */
    SESSION_MESSAGE_MAX = 0x2F,   /** Session��Ϣ���͵����ֵ */
    
    /***
      * Service��Ϣȡֵ��Χ
      */
    SERVICE_MESSAGE_MIN = 0x2F,   /** Service��Ϣ���͵���Сֵ */
    SERVICE_MESSAGE_MAX = 0x3F,   /** Service��Ϣ���͵����ֵ */   

    /***
      * Session��Ϣ����
      */
    MOOON_MESSAGE_SESSION_REQUEST,  /** Session������Ϣ */
    MOOON_MESSAGE_SESSION_RESPONSE, /** Session��Ӧ��Ϣ */

    /***
      * Service��Ϣ����
      */
    MOOON_MESSAGE_SERVICE_REQUEST,         /** Service������Ϣ */
    MOOON_MESSAGE_SERVICE_RESPONSE,        /** Service��Ӧ��Ϣ */
    MOOON_MESSAGE_SERVICE_ACTIVATE,        /** ����Service��Ϣ */
    MOOON_MESSAGE_SERVICE_DEACTIVATE,      /** ȥ����Service��Ϣ */
    MOOON_MESSAGE_SERVICE_CREATE_SESSION,  /** ����Session��Ϣ */
    MOOON_MESSAGE_SERVICE_DESTROY_SESSION  /** ����Session��Ϣ */    
}mooon_message_type_t;

/***
  * ͷ�ĸ��ֽ�����
  */
typedef struct first_four_bytes_t
{    
    uint32_t byte_order:1;  /** �ֽ��� */
    uint32_t total_size:26; /** �����ܴ�С����������ͷ�ĸ��ֽڣ����ֵΪ0x3FFFFFF */
    uint32_t padding:5;     /** ��䣬������չ�� */

    void zero()
    {
        *((uint32_t*)this) = 0;
    }

    void ntoh()
    {
        hton();
    }

    void hton()
    {
        // ֻ�е�ΪС�ֽ���ʱ������Ҫת����
        if (net::CNetUtil::is_little_endian())
        {            
            uint32_t byte_order_reversed = 0;
            *((uint32_t*)this) = net::CNetUtil::reverse_bytes(this, &byte_order_reversed, sizeof(*this));
        }
    }
}first_four_bytes_t;

/***
  * mooon��������
  */
typedef struct
{
    uint32_t ip[4];         /** �����IPV4��ID����IP[1~3]ֵΪ0*/
    uint16_t port;          /** ���ڵĶ˿ں� */
    uint16_t service_id;    /** Service ID */
    uint32_t session_id:30; /** Session ID��ֻ��Session��Ч */
    uint32_t ip_type:2;     /** IP���� */
    uint64_t timestamp;     /** ʱ�����ֻ��Session��Ч */

    /***
      * �����ֽ���ת���������ֽ���
      * ����ǰ��Ӧ�����ж��Ƿ���Ҫ�����ֽ���ת��
      */
    void hton()
    {

    }

    /***
      * �����ֽ���ת���������ֽ���
      * ����ǰ��Ӧ�����ж��Ƿ���Ҫ�����ֽ���ת��
      */
    void ntoh()
    {

    }
}mooon_t;

/***
  * ������Ϣ�ṹ
  */
typedef struct
{    
    uint32_t byte_order:1;  /** �ֽ��� */
    uint32_t type:7;        /** ������Ϣ���ͣ�ȡֵΪmooon_message_type_t�����ֵΪ0x7F */
    uint32_t size:24;       /** ��Ϣ�Ĵ�С��������schedule_message_t�������ֵΪ0xFFFFFF */
    char data[0];           /** �������Ϣ */

    /***
      * �����ֽ���ת���������ֽ���
      * ����ǰ��Ӧ�����ж��Ƿ���Ҫ�����ֽ���ת��
      */
    void hton()
    {
        uint32_t byte_order_reversed = 0;
        *((uint32_t*)this) = net::CNetUtil::reverse_bytes(this, &byte_order_reversed, sizeof(*this));
    }

    /***
      * �����ֽ���ת���������ֽ���
      * ����ǰ��Ӧ�����ж��Ƿ���Ҫ�����ֽ���ת��
      */
    void ntoh()
    {
        hton();
    }
}schedule_message_t;

/***
  * mooon��Ϣ�ṹ
  */
typedef struct
{
    mooon_t src_mooon;  /** Դmooon */
    mooon_t dest_mooon; /** Ŀ��mooon */
    char data[0];       /** ��Ϣ�����ݲ��� */

    /***
      * �����ֽ���ת���������ֽ���
      * ����ǰ��Ӧ�����ж��Ƿ���Ҫ�����ֽ���ת��
      */
    void hton()
    {
        src_mooon.hton();
        dest_mooon.hton();
    }

    /***
      * �����ֽ���ת���������ֽ���
      * ����ǰ��Ӧ�����ж��Ƿ���Ҫ�����ֽ���ת��
      */
    void ntoh()
    {
        src_mooon.ntoh();
        dest_mooon.ntoh();
    }
}mooon_message_t;

/***
  * �ж���Ϣ���ͺ���
  */
bool is_mooon_message(schedule_message_t* schedule_message);
bool is_service_message(schedule_message_t* schedule_message);
bool is_session_message(schedule_message_t* schedule_message);

#pragma pack()
MOOON_NAMESPACE_END
#endif // MOOON_SCHEDULER_MESSAGE_H
