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
 * Author: jian yi, eyjian@qq.com
 * 
 * NetInt��һ��ר���������ϴ�����������ͣ�
 * ʹ�����ĺô����������ֽ������⣬�������������һ��ʹ�������磺
 * struct Message
 * {
 *     NInt16 a;
 *     NInt16 b;
 *     NInt32 x;
 *     NInt32 y;
 *     NInt64 z;
 * };
 * ����ֱ�ӵ���send��Messageͨ�����緢�ͳ�ȥ���������������ֽ����ת����
 * �����յ���Ҳ����Ҫ��ת��������ֱ��ʹ�ã������Զ���ʽ��ת���������ֽ���
 *
 * ��������˺�BIG_NET_BYTES_ORDER���������ֽ���ʹ�ô��ֽ���
 * �������û�ж����BIG_NET_BYTES_ORDER���������ֽ���ʹ��С�ֽ���
 * Ĭ��û�ж����BIG_NET_BYTES_ORDER��Ҳ����Ĭ�������ֽ���ΪС�ֽ���
 */
#ifndef MOOON_NET_INTTYPES_H
#define MOOON_NET_INTTYPES_H
#include <net/util.h>
#include <util/bit_util.h>
NET_NAMESPACE_BEGIN

template <typename RawInt>
struct NetInt
{
public:
    NetInt()
     :_m(0)
    {
    }
    
    NetInt(RawInt m)
    {
        _m = transform(m);
    }
    
    NetInt& operator =(RawInt m)
    {
        _m = transform(m);
        return *this;
    }
    
    operator RawInt() const
    {
        return transform(_m);
    }
    
    RawInt to_int() const
    {
        return transform(_m);
    }
    
private:
    static RawInt transform(RawInt old_int)
    {
        RawInt new_int = 0;
        
        if (CUtil::is_little_endian())
        {
#ifndef BIG_NET_BYTES_ORDER
            new_int = old_int;
#else
            new_int = CUtil::reverse_bytes(old_int);
#endif
        }
        else
        {
#ifdef BIG_NET_BYTES_ORDER
            new_int = old_int;
#else
            new_int = CUtil::reverse_bytes(old_int);
#endif
        }
        
        return new_int;
    }
    
private:    
    NetInt(const NetInt&);

private:
    RawInt _m;
};

/***
  * ����������N��ͷ��N��Net����д
  */
typedef int8_t NInt8;
typedef uint8_t UNInt8;
typedef NetInt<int16_t> NInt16;
typedef NetInt<int32_t> NInt32;
typedef NetInt<int64_t> NInt64;
typedef NetInt<uint16_t> NUInt16;
typedef NetInt<uint32_t> NUInt32;
typedef NetInt<uint64_t> NUInt64;

typedef NInt8 nint8_t;
typedef NInt16 nint16_t;
typedef NInt32 nint32_t;
typedef NInt64 nint64_t;
typedef UNInt8 nuint8_t;
typedef UNInt16 nuint16_t;
typedef UNInt32 nuint32_t;
typedef UNInt64 nuint64_t;

NET_NAMESPACE_END
/***
  * ������ȫ�����ֿռ䣬�Լ�ʹ�ã�
  * ������N��ͷ������Ϊ������ȫ�����ֿռ��ڵ����ֳ�ͻ
  */
using net::NInt8;
using net::NInt16;
using net::NInt32;
using net::NInt64;
using net::NUInt8;
using net::NUInt16;
using net::NUInt32;
using net::NUInt64;

using net::nint8_t;
using net::nint16_t;
using net::nint32_t;
using net::nint64_t;
using net::nuint8_t;
using net::nuint16_t;
using net::nuint32_t;
using net::nuint64_t;
#endif // MOOON_NET_INTTYPES_H
