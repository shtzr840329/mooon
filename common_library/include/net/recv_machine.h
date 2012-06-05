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
#ifndef MOOON_NET_RECV_MACHINE_H
#define MOOON_NET_RECV_MACHINE_H
#include <net/config.h>
NET_NAMESPACE_BEGIN

/***
  * @MessageHeaderType ��Ϣͷ���ͣ�Ҫ���ǹ̶���С��
  * @ProcessorManager �ܹ����ָ����Ϣ���д�����࣬Ϊʲôȡ��ΪManager��
  *                   ��Ϊͨ����ͬ����Ϣ�ɲ�ͬ��Processor����
  */
template <typename MessageHeaderType, class ProcessorManager>
class CRecvMachine
{
private:
    /***
      * ����״ֵ̬
      */
    typedef enum TRecvState
    {
        rs_header = 0, /** ������Ϣͷ״̬ */
        rs_body   = 1 /** ������Ϣ��״̬ */
    }recv_state_t;

    /***
      * ����״̬������
      */
    struct RecvStateContext
    {
        const char* buffer; /** ��ǰ������buffer */
        size_t buffer_size; /** ��ǰ�������ֽ��� */

        RecvStateContext(const char* buf=NULL, size_t buf_size=0)
         :buffer(buf)
         ,buffer_size(buf_size)
        {
        }

        RecvStateContext(const RecvStateContext& other)
         :buffer(other.buffer)
         ,buffer_size(other.buffer_size)
        {
        }

        RecvStateContext& operator =(const RecvStateContext& other)
        {
            buffer = other.buffer;
            buffer_size = other.buffer_size;
            return *this;
        }
    };

public:
    CRecvMachine(ProcessorManager* processor_manager);
    util::handle_result_t work(const char* buffer, size_t buffer_size);
    void reset();

private:
    void set_next_state(recv_state_t next_state);
    util::handle_result_t handle_header(const RecvStateContext& cur_ctx, RecvStateContext* next_ctx);
    util::handle_result_t handle_body(const RecvStateContext& cur_ctx, RecvStateContext* next_ctx);
    util::handle_result_t handle_error(const RecvStateContext& cur_ctx, RecvStateContext* next_ctx);

private:
    MessageHeaderType _header; /** ��Ϣͷ�������С�ǹ̶��� */
    ProcessorManager* _processor_manager;
    recv_state_t _current_recv_state; /** ��ǰ�Ľ���״̬ */
    size_t _finished_size; /** ��ǰ״̬�Ѿ����յ����ֽ�����ע�ⲻ���ܵ��Ѿ����յ����ֽ�����ֻ��Ե�ǰ״̬ */
};

template <typename MessageHeaderType, class ProcessorManager>
CRecvMachine<MessageHeaderType, ProcessorManager>::CRecvMachine(ProcessorManager* processor_manager)
 :_processor_manager(processor_manager)
{
}

// ״̬����ں���
// ״̬������ԭ��-> rs_header -> rs_body -> rs_header
//                 -> rs_header -> rs_error -> rs_header
//                 -> rs_header -> rs_body -> rs_error -> rs_header
// ����˵����
// buffer - �����յ������ݣ�ע�ⲻ���ܵ�
// buffer_size - �����յ��������ֽ���
template <typename MessageHeaderType, class ProcessorManager>
util::handle_result_t CRecvMachine<MessageHeaderType, ProcessorManager>::work(
    const char* buffer, 
    size_t buffer_size)
{
    RecvStateContext next_ctx(buffer, buffer_size);
    util::handle_result_t hr = util::handle_continue;

    // ״̬��ѭ������Ϊ��util::handle_continue == hr
    while (util::handle_continue == hr)
    {
        RecvStateContext cur_ctx(next_ctx);

        switch (_current_recv_state)
        {
        case rs_header:
            hr = handle_header(cur_ctx, &next_ctx);
            break;
        case rs_body:
            hr = handle_body(cur_ctx, &next_ctx);
            break;
        default:
            hr = handle_error(cur_ctx, &next_ctx);
            break;
        }
    }

    return hr;
}

template <typename MessageHeaderType, class ProcessorManager>
void CRecvMachine<MessageHeaderType, ProcessorManager>::set_next_state(
    recv_state_t next_state)
{
    _current_recv_state = next_state;
    _finished_size = 0;
}

template <typename MessageHeaderType, class ProcessorManager>
void CRecvMachine<MessageHeaderType, ProcessorManager>::reset()
{
    set_next_state(rs_header);
}

// ������Ϣͷ��
// ����˵����
// cur_ctx - ��ǰ�����ģ�
//           cur_ctx.bufferΪ��ǰ�յ�������buffer����������Ϣͷ����Ҳ���ܰ�������Ϣ�塣
//           cur_ctx.buffer_sizeΪ��ǰ�յ��ֽ���
// next_ctx - ��һ�������ģ�
//           ����cur_ctx.buffer���ܰ�������Ϣ�壬������һ�ν���receive������
//           ���漰����Ϣͷ����Ϣ������״̬�������next_ctxʵ��Ϊ��һ��handle_body��cur_ctx
template <typename MessageHeaderType, class ProcessorManager>
util::handle_result_t CRecvMachine<MessageHeaderType, ProcessorManager>::handle_header(
    const RecvStateContext& cur_ctx, 
    RecvStateContext* next_ctx)
{
    if (_finished_size + cur_ctx.buffer_size < sizeof(MessageHeaderType))
    {
        memcpy(reinterpret_cast<char*>(&_header) + _finished_size
              ,cur_ctx.buffer
              ,cur_ctx.buffer_size);

        _finished_size += cur_ctx.buffer_size;
        return util::handle_continue;
    }
    else
    {
        size_t need_size = sizeof(MessageHeaderType) - _finished_size;
        memcpy(reinterpret_cast<char*>(&_header) + _finished_size
              ,cur_ctx.buffer
              ,need_size);

        // TODO: Check header here

        size_t remain_size = cur_ctx.buffer_size - need_size;
        if (remain_size > 0)
        {
            next_ctx->buffer = cur_ctx.buffer + need_size;
            next_ctx->buffer_size = cur_ctx.buffer_size - need_size;
        }

        // ֻ�е�������Ϣ��ʱ������Ҫ����״̬�л���
        // ����ά��rs_header״̬����
        if (_header.size > 0)
        {
            // �л�״̬
            set_next_state(rs_body);
        }
        else
        {
            if (!_processor_manager->on_message(_header, 0, NULL, 0))
            {
                return util::handle_error;
            }
        }

        return (remain_size > 0)
              ? util::handle_continue // ����work�����Ƿ����ѭ��
              : util::handle_finish;
    }
}

// ������Ϣ��
// ����˵����
// cur_ctx - ��ǰ�����ģ�
//           cur_ctx.bufferΪ��ǰ�յ�������buffer����������Ϣ�壬��Ҳ���ܰ�������Ϣͷ��
//           cur_ctx.buffer_sizeΪ��ǰ�յ��ֽ���
// next_ctx - ��һ�������ģ�
//           ����cur_ctx.buffer���ܰ�������Ϣͷ��������һ�ν���receive������
//           ���漰����Ϣͷ����Ϣ������״̬�������next_ctxʵ��Ϊ��һ��handle_header��cur_ctx
template <typename MessageHeaderType, class ProcessorManager>
util::handle_result_t CRecvMachine<MessageHeaderType, ProcessorManager>::handle_body(
    const RecvStateContext& cur_ctx, 
    RecvStateContext* next_ctx)
{
    if (_finished_size + cur_ctx.buffer_size < _header.size)
    {
        if (!_processor_manager->on_message(_header, _finished_size, cur_ctx.buffer, cur_ctx.buffer_size))
        {
            return util::handle_error;
        }

        _finished_size += cur_ctx.buffer_size;
        return util::handle_continue;
    }
    else
    {
        size_t need_size = _header.size - _finished_size;
        if (!_processor_manager->on_message(_header, _finished_size, cur_ctx.buffer, need_size))
        {
            return util::handle_error;
        }

        // �л�״̬
        set_next_state(rs_header);

        size_t remain_size = cur_ctx.buffer_size - need_size;
        if (remain_size > 0)
        {
            next_ctx->buffer = cur_ctx.buffer + need_size;
            next_ctx->buffer_size = cur_ctx.buffer_size - need_size;
            return util::handle_continue;
        }

        return util::handle_finish;
    }
}

template <typename MessageHeaderType, class ProcessorManager>
util::handle_result_t CRecvMachine<MessageHeaderType, ProcessorManager>::handle_error(
    const RecvStateContext& cur_ctx, 
    RecvStateContext* next_ctx)
{
    set_next_state(rs_header); // �������л���rs_header�����ʱ��Ӧ���Ͽ�����������
    return util::handle_error;
}

NET_NAMESPACE_END
#endif // MOOON_NET_RECV_MACHINE_H
