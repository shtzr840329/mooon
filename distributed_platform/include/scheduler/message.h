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
  * mooon��Ϣ����
  */
typedef enum
{
    MOOON_MESSAGE_MIN =0, /** ��Ϣ���Ϳ�ȡ����Сֵ */
    MOOON_MESSAGE_MAX =0, /** ��Ϣ���Ϳ�ȡ�����ֵ */

    /***
      * Service��Ϣ����
      */
    MOOON_MESSAGE_SERVICE_REQUEST,         /** Service������Ϣ */
    MOOON_MESSAGE_SERVICE_RESPONSE,        /** Service��Ӧ��Ϣ */
    MOOON_MESSAGE_SERVICE_ACTIVATE,        /** ����Service��Ϣ */
    MOOON_MESSAGE_SERVICE_DEACTIVATE,      /** ȥ����Service��Ϣ */
    MOOON_MESSAGE_SERVICE_CREATE_SESSION,  /** ����Session��Ϣ */
    MOOON_MESSAGE_SERVICE_DESTROY_SESSION, /** ����Session��Ϣ */
    /***
      * Session��Ϣ����
      */
    MOOON_MESSAGE_SESSION_REQUEST,  /** Session������Ϣ */
    MOOON_MESSAGE_SESSION_RESPONSE  /** Session��Ӧ��Ϣ */
}mooon_message_type_t;

/***
  * ͷ�ĸ��ֽ�����
  */
typedef struct first_four_bytes_t
{
    uint32_t byte_order:1;   /** �ֽ��� */
    uint32_t total_size:24;  /** �����ܴ�С����������ͷ�ĸ��ֽ� */
    uint32_t padding:7;      /** ��䣬������չ�� */

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
        if (BYTE_ORDER_LITTLE_ENDIAN == byte_order)
        {
            uint32_t byte_order_reversed = 0;

            net::CNetUtil::reverse_bytes(this, &byte_order_reversed, sizeof(*this));
            *((uint32_t*)this) = byte_order_reversed;
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
}mooon_t;

/***
  * mooon��Ϣ�ṹ
  */
typedef struct
{    
    uint32_t type:8;
    uint32_t size:24;
    mooon_t src_mooon;
    mooon_t dest_moooon;
    char data[0];
}mooon_message_t;

#pragma pack()
MOOON_NAMESPACE_END
#endif // MOOON_SCHEDULER_MESSAGE_H
