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
#ifndef MOOON_UTIL_INTEGER_UTIL_H
#define MOOON_UTIL_INTEGER_UTIL_H
#include <math.h>
#include "util/config.h"
UTIL_NAMESPACE_BEGIN

/***
  * 整数数字操作工具类
  */
class CIntegerUtil
{
public:	
	template <typename DataType>
	static bool is_prime_number(DataType x)
	{
		if (x < 2)
			return false;

		DataType k = (DataType)sqrt(x);
		for (DataType m=2; m<=k; ++m)
			if (0 == x % m)
				return false;

		return true;
	}

    /** 判断一个数字是否可为int16_t数字 */
    static bool is_int16(int32_t num);

    /** 判断一个数字是否可为uint16_t数字 */
    static bool is_uint16(int32_t num);
    static bool is_uint16(uint32_t num);

    /** 判断一个数字是否可为int32_t数字 */
    static bool is_int32(int64_t num);

    /** 判断一个数字是否可为uint32_t数字 */
    static bool is_uint32(int64_t num);
    static bool is_uint32(uint64_t num);
};

UTIL_NAMESPACE_END
#endif // MOOON_UTIL_INTEGER_UTIL_H
