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
 * Author: JianYi, eyjian@qq.com or eyjian@gmail.com
 */
#ifndef UTIL_ARRAY_QUEUE_H
#define UTIL_ARRAY_QUEUE_H
#include "util/util_config.h"
UTIL_NAMESPACE_BEGIN

/** 用数组实现的队列, 非线程安全 */
template <typename DataType>
class CArrayQueue
{       
public:
    typedef DataType _DataType;
	CArrayQueue(uint32_t queue_max)
		:_tail(0)
		,_head(0)
		,_queue_size(0)
    {
        _queue_max = queue_max+1;
		_elem_array = new DataType[queue_max];        
        memset(_elem_array, 0, queue_max);
    }
	
    ~CArrayQueue()
    {
        delete []_elem_array;
    }
        
    bool is_full() const 
	{
        return ((_tail+1) % _queue_max == _head);
    }
    
    bool is_empty() const 
	{
        return (_head == _tail);
    }

    DataType front() const 
	{
        return _elem_array[_head];
    }
    
	/** 调用pop之前应当先使用is_empty判断一下 */
    DataType pop_front() 
	{
        DataType elem = _elem_array[_head];
        _head = (_head+1) % _queue_max;
        --_queue_size;
        return elem;
    }
    
	/** 调用pop之前应当先使用is_full判断一下 */
    void push_back(DataType elem) 
	{
        _elem_array[_tail] = elem;
        _tail = (_tail+1) % _queue_max; 
        ++_queue_size;
    }

    uint32_t size() const 
	{ 
		return _queue_size; 
	}
    
private:        
    uint32_t _tail;       /** 队首 */
    uint32_t _head;       /** 队尾 */    
    uint32_t _queue_size; /** 队列当前容纳的元素个数 */	
	uint32_t _queue_max;  /** 队列最多可容纳的元素个数 */	
	DataType* _elem_array; /** 用来实现队列的数组 */
};

UTIL_NAMESPACE_END
#endif // UTIL_ARRAY_QUEUE_H
