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
 */
//#include <alloca.h>
#include <limits>
#include "util/string_util.h"
UTIL_NAMESPACE_BEGIN

/***
  * �����ַ���ת��������ģ��ͨ�ú���
  * @str: ��Ҫ��ת�����ַ���
  * @result: �洢ת����Ľ��
  * @max_length: ���������Ͷ�Ӧ���ַ���������ַ���������������β��
  * @converted_length: ��Ҫת�����ַ������ȣ����Ϊ0���ʾת�������ַ���
  * @ignored_zero: �Ƿ���Կ�ͷ��0
  * @return: ���ת���ɹ�����true, ���򷵻�false
  */
template <typename IntType>
static bool fast_string2int(const char* str, IntType& result, uint8_t max_length, uint8_t converted_length, bool ignored_zero)
{
    bool negative = false;
    const char* tmp_str = str;
    if (NULL == str) return false;

    // ������
    if ('-' == tmp_str[0])
    {
        // ����
        negative = true;
        ++tmp_str;
    }

    // ������ַ���
    if ('\0' == tmp_str[0]) return false;

    // ����0��ͷ��
    if ('0' == tmp_str[0])
    {
        // �����0��ͷ����ֻ����һλ����
        if ('\0' == tmp_str[1])
        {
            result = 0;
            return true;
        }
        else
        {
            if (!ignored_zero) return false;
            for (;;)
            {
                ++tmp_str;
                if (tmp_str - str > max_length-1) return false;
                if (*tmp_str != '0') break;
            }
            if ('\0' == *tmp_str)
            {
                result = 0;
                return true;
            }
        }
    }

    // ����һ���ַ�
    if ((*tmp_str < '0') || (*tmp_str > '9')) return false;
    result = (*tmp_str - '0');

    while ((0 == converted_length) || (tmp_str - str < converted_length-1))
    {
        ++tmp_str;
        if ('\0' == *tmp_str) break;
        if (tmp_str - str > max_length-1) return false;

        if ((*tmp_str < '0') || (*tmp_str > '9')) return false;

        result = result * 10;
        result += (*tmp_str - '0');
    }

    if (negative)
        result = -result;
    return true;
}

void CStringUtil::remove_last(std::string& source, char c)
{
    std::string::size_type pos = source.rfind(c);
    if (pos+1 == source.length())
        source.erase(pos);
}

void CStringUtil::remove_last(std::string& source, const std::string& sep)
{
    // std: $HOME/bin/exe
    // sep: /bin/
    // ---> $HOME
    std::string::size_type pos = source.rfind(sep);
    if (pos != std::string::npos)
        source.erase(pos);
}

void CStringUtil::to_upper(char* source)
{
    char* tmp_source = source;
    while (*tmp_source != '\0')
    {
        if ((*tmp_source >= 'a') && (*tmp_source <= 'z'))
            *tmp_source += 'A' - 'a';

        ++tmp_source;
    }
}

void CStringUtil::to_lower(char* source)
{
    char* tmp_source = source;
    while (*tmp_source != '\0')
    {
        if ((*tmp_source >= 'A') && (*tmp_source <= 'Z'))
            *tmp_source += 'a' - 'A';

        ++tmp_source;
    }
}

void CStringUtil::to_upper(std::string& source)
{
    // ֻ�޸Ĵ�Сд������������
    char* tmp_source = (char *)source.c_str();
    to_upper(tmp_source);
}

void CStringUtil::to_lower(std::string& source)
{
    // ֻ�޸Ĵ�Сд������������
    char* tmp_source = (char *)source.c_str();
    to_lower(tmp_source);
}

/** �ж�ָ���ַ��Ƿ�Ϊ�ո��TAB��(\t)��س���(\r)���з�(\n) */
bool CStringUtil::is_space(char c)
{
    return (' ' == c) || ('\t' == c) || ('\r' == c) || ('\n' == c);
}

// ��ʹ��trim_left��trim_right���ʵ�֣��Ա���Ч��
void CStringUtil::trim(char* source)
{
    char* space = NULL;
    char* tmp_source = source;
    while (' ' == *tmp_source) ++tmp_source;

    for (;;)
    {
        *source = *tmp_source;
        if ('\0' == *tmp_source)
        {
            if (space != NULL)
                *space = '\0';
            break;
        }
        else if (is_space(*tmp_source))
        {
            if (NULL == space)
                space = source;
        }
        else
        {
            space = NULL;
        }

        ++source;
        ++tmp_source;
    }
}

void CStringUtil::trim_left(char* source)
{
    char* tmp_source = source;
    while (is_space(*tmp_source)) ++tmp_source;

    for (;;)
    {
        *source = *tmp_source;
        if ('\0' == *tmp_source) break;

        ++source;
        ++tmp_source;
    }
}

void CStringUtil::trim_right(char* source)
{
    char* space = NULL;
    char* tmp_source = source;

    for (;;)
    {
        if ('\0' == *tmp_source)
        {
            if (space != NULL)
                *space = '\0';
            break;
        }
        else if (is_space(*tmp_source))
        {
            if (NULL == space)
                space = tmp_source;
        }
        else
        {
            space = NULL;
        }

        ++tmp_source;
    }
}

void CStringUtil::trim(std::string& source)
{
    trim_left(source);
    trim_right(source);
}

void CStringUtil::trim_left(std::string& source)
{
    // ����ֱ�Ӷ�c_str()�����޸ģ���Ϊ���ȷ����˱仯
    size_t length = source.length();
    char* tmp_source = new char[length+1];
    delete_helper<char> dh(tmp_source);

    strncpy(tmp_source, source.c_str(), length);
    tmp_source[length] = '\0';

    trim_left(tmp_source);
    source = tmp_source;
}

void CStringUtil::trim_right(std::string& source)
{
    // ����ֱ�Ӷ�c_str()�����޸ģ���Ϊ���ȷ����˱仯
    size_t length = source.length();
    char* tmp_source = new char[length+1];
    delete_helper<char> dh(tmp_source);

    strncpy(tmp_source, source.c_str(), length);
    tmp_source[length] = '\0';

    trim_right(tmp_source);
    source = tmp_source;
}

bool CStringUtil::string2int8(const char* source, int8_t& result, uint8_t converted_length, bool ignored_zero)
{
    return string2int(source, result, converted_length, ignored_zero);
}

bool CStringUtil::string2int(const char* source, int8_t& result, uint8_t converted_length, bool ignored_zero)
    int16_t value = 0;

    if (!string2int16(source, value, converted_length, ignored_zero)) return false;
    if (value < std::numeric_limits<int8_t>::min() 
     || value > std::numeric_limits<int8_t>::max()) return false;
    
    result = (int8_t)value;
    return true;
}

bool CStringUtil::string2int16(const char* source, int16_t& result, uint8_t converted_length, bool ignored_zero)
{
    return string2int(source, result, converted_length, ignored_zero);
}

bool CStringUtil::string2int(const char* source, int16_t& result, uint8_t converted_length, bool ignored_zero)
{
    int32_t value = 0;

    if (!string2int32(source, value, converted_length, ignored_zero)) return false;
    if (value < std::numeric_limits<int16_t>::min()
     || value > std::numeric_limits<int16_t>::max()) return false;

    result = (int16_t)value;
    return true;
}

bool CStringUtil::string2int32(const char* source, int32_t& result, uint8_t converted_length, bool ignored_zero)
{
    return string2int(source, result, converted_length, ignored_zero);
}

bool CStringUtil::string2int(const char* source, int32_t& result, uint8_t converted_length, bool ignored_zero)
{
    if (NULL == source) return false;

    long value;
    if (!fast_string2int<long>(source, value, sizeof("-2147483648")-1, converted_length, ignored_zero)) return false;
    if ((value < std::numeric_limits<int32_t>::min())
     || (value > std::numeric_limits<int32_t>::max()))  return false;

    result = (int32_t)value;
    return true;
}

bool CStringUtil::string2int64(const char* source, int64_t& result, uint8_t converted_length, bool ignored_zero)
{
    return string2int(source, result, converted_length, ignored_zero);
}

bool CStringUtil::string2int(const char* source, int64_t& result, uint8_t converted_length, bool ignored_zero)
{
    long long value;
    if (!fast_string2int<long long>(source, value, sizeof("-9223372036854775808")-1, converted_length, ignored_zero)) return false;

    result = (int64_t)value;
    return true;
}

bool CStringUtil::string2uint8(const char* source, uint8_t& result, uint8_t converted_length, bool ignored_zero)
{
    return string2int(source, result, converted_length, ignored_zero);
}

bool CStringUtil::string2int(const char* source, uint8_t& result, uint8_t converted_length, bool ignored_zero)
{
    uint16_t value = 0;
    if (!string2uint16(source, value, converted_length, ignored_zero)) return false;
    if (value > std::numeric_limits<uint8_t>::max()) return false;

    result = (uint8_t)value;
    return true;
}

bool CStringUtil::string2uint16(const char* source, uint16_t& result, uint8_t converted_length, bool ignored_zero)
{
    return string2int(source, result, converted_length, ignored_zero);
}

bool CStringUtil::string2int(const char* source, uint16_t& result, uint8_t converted_length, bool ignored_zero)
{
    uint32_t value = 0;
    if (!string2uint32(source, value, converted_length, ignored_zero)) return false;
    if (value > std::numeric_limits<uint16_t>::max()) return false;

    result = (uint16_t)value;
    return true;
}

bool CStringUtil::string2uint32(const char* source, uint32_t& result, uint8_t converted_length, bool ignored_zero)
{
    return string2int(source, result, converted_length, ignored_zero);
}

bool CStringUtil::string2int(const char* source, uint32_t& result, uint8_t converted_length, bool ignored_zero)
{
    unsigned long value;
    if (!fast_string2int<unsigned long>(source, value, sizeof("4294967295")-1, converted_length, ignored_zero)) return false;

    result = (uint32_t)value;
    return true;
}

bool CStringUtil::string2uint64(const char* source, uint64_t& result, uint8_t converted_length, bool ignored_zero)
{
    return string2int(source, result, converted_length, ignored_zero);
}

bool CStringUtil::string2int(const char* source, uint64_t& result, uint8_t converted_length, bool ignored_zero)
{
    unsigned long long value;
    if (!fast_string2int<unsigned long long>(source, value, sizeof("18446744073709551615")-1, converted_length, ignored_zero)) return false;

    result = (uint64_t)value;
    return true;
}

std::string CStringUtil::int16_tostring(int16_t source)
{
    return int_tostring(source);
}

std::string CStringUtil::int_tostring(int16_t source)
{
    char str[sizeof("065535")]; // 0xFFFF
    snprintf(str, sizeof(str), "%d", source);
    return str;
}

std::string CStringUtil::int32_tostring(int32_t source)
{
    return int_tostring(source);
}

std::string CStringUtil::int_tostring(int32_t source)
{
    char str[sizeof("04294967295")]; // 0xFFFFFFFF
    snprintf(str, sizeof(str), "%d", source);
    return str;
}

std::string CStringUtil::int64_tostring(int64_t source)
{
    return int_tostring(source);
}

std::string CStringUtil::int_tostring(int64_t source)
{
    char str[sizeof("018446744073709551615")]; // 0xFFFFFFFFFFFFFFFF
#if __WORDSIZE==64
    snprintf(str, sizeof(str), "%ld", source);
#else
    snprintf(str, sizeof(str), "%lld", source);
#endif
    return str;
}

std::string CStringUtil::uint16_tostring(uint16_t source)
{
    return int_tostring(source);
}

std::string CStringUtil::int_tostring(uint16_t source)
{
    char str[sizeof("065535")]; // 0xFFFF
    snprintf(str, sizeof(str), "%u", source);
    return str;
}

std::string CStringUtil::uint32_tostring(uint32_t source)
{
    return int_tostring(source);
}

std::string CStringUtil::int_tostring(uint32_t source)
{
    char str[sizeof("04294967295")]; // 0xFFFFFFFF
    snprintf(str, sizeof(str), "%u", source);
    return str;
}

std::string CStringUtil::uint64_tostring(uint64_t source)
{
    return int_tostring(source);
}

std::string CStringUtil::int_tostring(uint64_t source)
{
    char str[sizeof("018446744073709551615")]; // 0xFFFFFFFFFFFFFFFF
#if __WORDSIZE==64
    snprintf(str, sizeof(str), "%lu", source);
#else
    snprintf(str, sizeof(str), "%llu", source);
#endif
    return str;
}

char* CStringUtil::skip_spaces(char* buffer)
{
    char* iter = buffer;
    while (' ' == *iter) ++iter;

    return iter;
}

const char* CStringUtil::skip_spaces(const char* buffer)
{
    const char* iter = buffer;
    while (' ' == *iter) ++iter;

    return iter;
}

uint32_t CStringUtil::hash(const char *str, int len)
{
    uint32_t g;
    uint32_t h = 0;
    const char *p = str;

    while (p < str+len)
    {
        h = (h << 4) + *p++;
        if ((g = (h & 0xF0000000)))
        {
            h = h ^ (g >> 24);
            h = h ^ g;
        }
    }

    return h;
}

int CStringUtil::fix_snprintf(char *str, size_t size, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    int expected = fix_vsnprintf(str, size, format, ap);
    va_end(ap);

    return expected;
}

int CStringUtil::fix_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    int expected = vsnprintf(str, size, format, ap);
    expected = (expected > ((int)size-1))? size-1: expected;
    return expected;
}

UTIL_NAMESPACE_END
