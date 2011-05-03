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
#include "request_responsor.h"
MOOON_NAMESPACE_BEGIN

CRequestResponsor::CRequestResponsor(CProtocolParser* parser)
    :_parser(parser)
{
}

/** ��λ״̬ */
void CRequestResponsor::reset();
{

}

/** �Ƿ񱣳����Ӳ��Ͽ���������һ������ */
bool CRequestResponsor::keep_alive() const;
{

}

/** �Ƿ���һ���ļ� */
bool CRequestResponsor::is_send_file() const;
{

}

/** �õ���Ҫ���͵Ĵ�С */
uint32_t CRequestResponsor::get_size() const;
{

}

/** �õ�����ƫ�ƿ�ʼ���� */
uint32_t CRequestResponsor::get_offset() const; 
{

}

/** �õ��ļ���� */
int CRequestResponsor::get_fd() const;              


/** �õ���Ҫ���͵����� */
const char* CRequestResponsor::get_buffer() const;
{

}

/***
  * �ƶ�ƫ��
  * @offset: ���η��͵��ֽ���
  */
void CRequestResponsor::move_offset(uint32_t offset)
{

}

MOOON_NAMESPACE_END
