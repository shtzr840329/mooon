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
#ifndef MOOON_RECEIVER_PROTOCOL_PARSER_H
#define MOOON_RECEIVER_PROTOCOL_PARSER_H
#include "server/protocol_parser.h"
MOOON_NAMESPACE_BEGIN

/***
  * Э�������
  */
class CProtocolParser: public IProtocolParser
{
public:
    CProtocolParser();
    ~CProtocolParser();

private:
    /***
      * ��λ����״̬
      */
    virtual void reset();

    /***
      * ���յ������ݽ��н���
      * @data_size: ���յ������ݴ�С
      */
    virtual util::handle_result_t parse(uint32_t data_size);

    /***
      * ����Bufferƫ��
      * @offset: �½��յ������ݴ�С
      */
    virtual void move_buffer_offset(uint32_t offset);

    /***
      * �õ����ĸ�λ�ÿ�ʼ�����յ������ݴ洢��Buffer
      */
    virtual uint32_t get_buffer_offset() const;

    /***
      * �õ������������ݵ�Buffer��С
      */
    virtual uint32_t get_buffer_size() const;    

    /***
      * �õ������������ݵ�Buffer
      */
    virtual char* get_buffer();

private:
    char* _packet;    
    bool _to_receive_packet; /** �Ƿ�ȥ��Ϣ�� */
    uint32_t _current_offset;
    first_four_bytes_t _first_four_bytes;
};

MOOON_NAMESPACE_END
#endif // MOOON_RECEIVER_PROTOCOL_PARSER_H
