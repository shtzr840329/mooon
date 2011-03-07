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
#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H
#include "sys/event.h"
SYS_NAMESPACE_BEGIN

/** 事件队列，总是线程安全
  * 特性1: 如果队列为空，则可等待队列有数据时
  * 特性2: 如果队列已满，则可等待队列为非满时
  * RawQueueClass为原始队列类名，如util::CArrayQueue
  */
template <class RawQueueClass>
class CEventQueue
{       
public:
    /** 队列中的元素数据类型 */
    typedef typename RawQueueClass::_DataType DataType;

    /***
      * 构造一个事件队列
      * @pop_milliseconds: pop_front时等待队列为非空时的毫秒数，如果为0则表示不等待，
      *                这种情况下如果队列为空，则pop_front立即返回false
      * @push_milliseconds: push_back时等待队列为非满时的毫秒数，如果为0则表示不等待，
      *                这种情况下如果队列已满，则push_back立即返回false
      * @queue_max: 需要构造的队列大小
      */
	CEventQueue(uint32_t queue_max, uint32_t pop_milliseconds, uint32_t push_milliseconds)
		:_raw_queue(queue_max)
        ,_pop_milliseconds(pop_milliseconds)
        ,_push_milliseconds(push_milliseconds)
        ,_pop_waiter_number(0)
        ,_push_waiter_number(0)
    {
    }

    /** 判断队列是否已满 */
    bool is_full() const 
	{
        CLockHelper<CLock> lock(_lock);
        return _raw_queue.is_full();
    }
    
    /** 判断队列是否为空 */
    bool is_empty() const 
	{
        CLockHelper<CLock> lock(_lock);
        return _raw_queue.is_empty();
    }
    
    /***
      * 返回队首元素
      * @elem: 存储队首元素
      * @return: 如果队列不为空则返回true，否则返回false
      */
    bool front(DataType& elem) const 
	{
        CLockHelper<CLock> lock(_lock);
        if (_raw_queue.is_empty()) return false;
        
        elem = _raw_queue.front();
        return true;
    }

	/***
      * 弹出队首元素
      * @elem: 存储被弹出的队首元素
      * @return: 如果成功从队列弹出数据则返回true，否则（队列为空或超时）返回false
      * @exception: 可能抛出CSyscallExceptoin异常，其它异常和RawQueue有关
      */
    bool pop_front(DataType& elem) 
	{
        CLockHelper<CLock> lock(_lock);
        while (_raw_queue.is_empty())
        {
            // 如果不等待，则立即返回
            if (0 == _pop_milliseconds) return false;
            // 使用助手类管理计数，因为timed_wait可能抛异常
            util::CountHelper<volatile int> ch(_pop_waiter_number); 
            
            // 超时则立即返回
            if (!_event.timed_wait(_lock, _pop_milliseconds)) return false;
        }

        elem = _raw_queue.pop_front();
        if (_push_waiter_number > 0) _event.signal();
        return true;
    }
    
	/***
      * 往队尾插入一个元素
      * @elem: 需要插入队尾的数据
      * @return: 如果成功往对尾插入了数据，则返回true，否则（队列满或超时）返回false
      * @exception: 可能抛出CSyscallExceptoin异常，其它异常和RawQueue有关
      */
    bool push_back(DataType elem) 
	{
        CLockHelper<CLock> lock(_lock);
        while (_raw_queue.is_full())
        {
            // 如果不等待，则立即返回
            if (0 == _push_milliseconds) return false;  
            // 使用助手类管理计数，因为timed_wait可能抛异常
            util::CountHelper<volatile int> ch(_push_waiter_number); 

            // 超时则立即返回
            if (!_event.timed_wait(_lock, _push_milliseconds)) return false;
        }

        _raw_queue.push_back(elem);
        if (_pop_waiter_number > 0) _event.signal();
        return true;
    }

    /** 得到队列中存储的元素个数 */
    uint32_t size() const 
	{ 
        CLockHelper<CLock> lock(_lock);
		return _raw_queue.size(); 
	}

private:        
    CEvent _event;
    mutable CLock _lock;    
    RawQueueClass _raw_queue;      /** 原始队列 */       

private:
    uint32_t _pop_milliseconds;    /** 出队时等待超时毫秒数 */
    uint32_t _push_milliseconds;   /** 入队时等待超时毫秒数 */
    volatile int _pop_waiter_number;  /** 等待队列有数据的线程个数 */
    volatile int _push_waiter_number; /** 等待队列有空位置的线程个数 */
};

SYS_NAMESPACE_END
#endif // EVENT_QUEUE_H
