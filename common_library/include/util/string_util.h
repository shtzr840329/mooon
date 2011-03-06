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
#ifndef STRING_UTIL_H
#define STRING_UTIL_H
#include <math.h>
#include "util/util_config.h"
UTIL_NAMESPACE_BEGIN

class CStringUtil
{
public:
	/** ɾ���ַ���β����ָ���ַ���ʼ������
	  * @source: ��Ҫ������ַ���
	  * @c: �ָ��ַ�
	  * @example:  ���strΪ��/usr/local/test/bin/������cΪ��/����
      *            �����str��ɡ�/usr/local/test/bin��
	  */
	static void remove_last(std::string& source, char c);

	/** ɾ���ַ���β����ָ���ַ�����ʼ������
	  * @source: ��Ҫ������ַ���
	  * @sep: �ָ��ַ���
	  * @example: ���strΪ��/usr/local/test/bin/tt������sepΪ��/bin/����
      *           �����str��ɡ�/usr/local/test
	  */
	static void remove_last(std::string& source, const std::string& sep);

    /** ���ַ����е�����Сд�ַ�ת���ɴ�д */
    static void to_upper(char* source);
    static void to_upper(std::string& source);

    /** ���ַ����е����д�д�ַ�ת����Сд */
    static void to_lower(char* source);    
    static void to_lower(std::string& source);

    /** �ж�ָ���ַ��Ƿ�Ϊ�ո��TAB��(\t)��س���(\r)���з�(\n) */
    static bool is_space(char c);
    
    /** ɾ���ַ�����β�ո��TAB��(\t)��س���(\r)���з�(\n) */
    static void trim(char* source);
    static void trim(std::string& source);

    /** ɾ���ַ����ײ��ո��TAB��(\t)��س���(\r)���з�(\n) */
    static void trim_left(char* source);
    static void trim_left(std::string& source);

    /** ɾ���ַ���β���ո��TAB��(\t)��س���(\r)���з�(\n) */
    static void trim_right(char* source);        
    static void trim_right(std::string& source);

	/**
	  * �ַ���ת������������
	  */
	
    /** ���ַ���ת����8λ���з�������
	  * @source: ��ת�����������ַ���
	  * @result: ת���������
      * @converted_length: ��Ҫ��������ת�����ַ��������������ֲ������������ȡֵΪ0���������ַ���
      * @ignored_zero: �Ƿ����������ַ�����0��ͷ�����Զ�����
	  * @return: ���ת���ɹ�����true�����򷵻�false
	  */
    static bool string2int(const char* source, int8_t& result, uint8_t converted_length=0, bool ignored_zero=false);
	static bool string2int8(const char* source, int8_t& result, uint8_t converted_length=0, bool ignored_zero=false);    

	/** ���ַ���ת����16λ���з�������
	  * @source: ��ת�����������ַ���
	  * @result: ת���������
      * @converted_length: ��Ҫ��������ת�����ַ��������������ֲ������������ȡֵΪ0���������ַ���
      * @ignored_zero: �Ƿ����������ַ�����0��ͷ�����Զ�����
	  * @return: ���ת���ɹ�����true�����򷵻�false
	  */
    static bool string2int(const char* source, int16_t& result, uint8_t converted_length=0, bool ignored_zero=false);
	static bool string2int16(const char* source, int16_t& result, uint8_t converted_length=0, bool ignored_zero=false);    

	/** ���ַ���ת����32λ���з�������
	  * @source: ��ת�����������ַ���
	  * @result: ת���������
      * @converted_length: ��Ҫ��������ת�����ַ��������������ֲ������������ȡֵΪ0���������ַ���
      * @ignored_zero: �Ƿ����������ַ�����0��ͷ�����Զ�����
	  * @return: ���ת���ɹ�����true�����򷵻�false
	  */
    static bool string2int(const char* source, int32_t& result, uint8_t converted_length=0, bool ignored_zero=false);
	static bool string2int32(const char* source, int32_t& result, uint8_t converted_length=0, bool ignored_zero=false);    

	/** ���ַ���ת����64λ���з�������
	  * @source: ��ת�����������ַ���
	  * @result: ת���������
      * @converted_length: ��Ҫ��������ת�����ַ��������������ֲ������������ȡֵΪ0���������ַ���
      * @ignored_zero: �Ƿ����������ַ�����0��ͷ�����Զ�����
	  * @return: ���ת���ɹ�����true�����򷵻�false
	  */
    static bool string2int(const char* source, int64_t& result, uint8_t converted_length=0, bool ignored_zero=false);
	static bool string2int64(const char* source, int64_t& result, uint8_t converted_length=0, bool ignored_zero=false);    

    /** ���ַ���ת����8λ���޷�������
	  * @source: ��ת�����������ַ���
	  * @result: ת���������
      * @converted_length: ��Ҫ��������ת�����ַ��������������ֲ������������ȡֵΪ0���������ַ���
      * @ignored_zero: �Ƿ����������ַ�����0��ͷ�����Զ�����
	  * @return: ���ת���ɹ�����true�����򷵻�false
	  */
    static bool string2int(const char* source, uint8_t& result, uint8_t converted_length=0, bool ignored_zero=false);
	static bool string2uint8(const char* source, uint8_t& result, uint8_t converted_length=0, bool ignored_zero=false);    

	/** ���ַ���ת����16λ���޷�������
	  * @source: ��ת�����������ַ���
	  * @result: ת���������
      * @converted_length: ��Ҫ��������ת�����ַ��������������ֲ������������ȡֵΪ0���������ַ���
      * @ignored_zero: �Ƿ����������ַ�����0��ͷ�����Զ�����
	  * @return: ���ת���ɹ�����true�����򷵻�false
	  */
    static bool string2int(const char* source, uint16_t& result, uint8_t converted_length=0, bool ignored_zero=false);
	static bool string2uint16(const char* source, uint16_t& result, uint8_t converted_length=0, bool ignored_zero=false);    

	/** ���ַ���ת����32λ���޷�������
	  * @source: ��ת�����������ַ���
	  * @result: ת���������
      * @converted_length: ��Ҫ��������ת�����ַ��������������ֲ������������ȡֵΪ0���������ַ���
      * @ignored_zero: �Ƿ����������ַ�����0��ͷ�����Զ�����
	  * @return: ���ת���ɹ�����true�����򷵻�false
	  */
    static bool string2int(const char* source, uint32_t& result, uint8_t converted_length=0, bool ignored_zero=false);
	static bool string2uint32(const char* source, uint32_t& result, uint8_t converted_length=0, bool ignored_zero=false);    

	/** ���ַ���ת����64λ���޷�������
	  * @source: ��ת�����������ַ���
	  * @result: ת���������
      * @converted_length: ��Ҫ��������ת�����ַ��������������ֲ������������ȡֵΪ0���������ַ���
      * @ignored_zero: �Ƿ����������ַ�����0��ͷ�����Զ�����
	  * @return: ���ת���ɹ�����true�����򷵻�false
	  */
    static bool string2int(const char* source, uint64_t& result, uint8_t converted_length=0, bool ignored_zero=false);
	static bool string2uint64(const char* source, uint64_t& result, uint8_t converted_length=0, bool ignored_zero=false);    

    static std::string int_tostring(int16_t source);
    static std::string int16_tostring(int16_t source);    

    static std::string int32_tostring(int32_t source);
    static std::string int_tostring(int32_t source);

    static std::string int_tostring(int64_t source);
    static std::string int64_tostring(int64_t source);

    static std::string int_tostring(uint16_t source);
    static std::string uint16_tostring(uint16_t source);    

    static std::string int_tostring(uint32_t source);
    static std::string uint32_tostring(uint32_t source);    

    static std::string int_tostring(uint64_t source);
    static std::string uint64_tostring(uint64_t source);    
    
    /** �����ո񲿷�
      */
    static char* skip_spaces(char* buffer);
    static const char* skip_spaces(const char* buffer);

    static uint32_t hash(const char *str, int len);

    /***
      * ��ͬ�ڱ�׼���snprintf�������snprintf���Ǳ�֤����ʵ����strд����ֽ���
      * ��������size�Ƿ��㹻���ɣ�������Ϊ��ͬ
      */
    static int fix_snprintf(char *str, size_t size, const char *format, ...);
    static int fix_vsnprintf(char *str, size_t size, const char *format, va_list ap);
};

UTIL_NAMESPACE_END
#endif // STRING_UTIL_H
