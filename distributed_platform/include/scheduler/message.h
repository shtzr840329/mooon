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
#ifndef MOOON_MESSAGE_H
#define MOOON_MESSAGE_H
MOOON_NAMESPACE_BEGIN
#pragma pack(4)

/***
  * ��������
  */
enum
{
    MAX_SERVICE_ID = 10000��  /** ����Service ID */
    MAX_SESSION_ID = 100000�� /** ����Session ID */
};

/***
  * mooon����Ψһ��ʶ�ṹ
  */
typedef struct
{
    uint32_t ip[4];     /** ���ڵ�IP��ַ */
    uint16_t port;      /** ���ڵĶ˿ں� */
    uint16_t service;   /** Service ID*/
    uint32_t session;   /** Session ID�����ֻ�Ǹ�Service�������ֵΪ0 */
    uint64_t timestamp; /** ʱ�����ȡ����ʱ��ʱ�� */
}moid_t;

/***
  * ��Ϣ����ͷ4���ֽ�
  */
struct
{
    uint32_t ipv4:1;        /** �Ƿ�ΪIPV4 */
    uint32_t byte_order:1;  /** �Ƿ�ΪС�ֽ��� */
    uint32_t total_size:24; /** ��Ϣ�����ܴ�С */
    uint32_t padding:6;     /** �����õ�6���� */
}first_four_bytes_t;

/***
  * ������Ϣ����
  */
typedef struct
{
    SCHEDULE_MESSAGE_GET_SESSION,       /** ��ServiceҪһ��Session */
    SCHEDULE_MESSAGE_RELEASE_SESSION,   /** �黹Session��Service */
    SCHEDULE_MESSAGE_SERVICE_REQUEST,   /** Service���� */
    SCHEDULE_MESSAGE_SERVICE_RESPONSE,  /** Service��Ӧ */
    SCHEDULE_MESSAGE_SESSION_REQUEST,   /** Session���� */
    SCHEDULE_MESSAGE_SESSION_RESPONSE   /** Session��Ӧ */
}schedule_message_type_t;

/***
  * ������Ϣ�ṹ
  */
typedef struct
{
    schedule_message_type_t type;
    moid_t src_moid;  /** Դmoid */
    moid_t dest_moid; /** Ŀ��moid */
    char data[0];     /** ������ */
}schedule_message_t;

#pragma pack()
MOOON_NAMESPACE_END
#endif // MOOON_MESSAGE_H
