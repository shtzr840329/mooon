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
#ifndef UTIL_CONFIG_H
#define UTIL_CONFIG_H
#include <string>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>
#include <stddef.h> // offsetof
//#include <limits> std::numeric_limits<>

/* �������ֿռ�� */
#define MOOON_NAMESPACE_BEGIN namespace mooon {
#define MOOON_NAMESPACE_END                   }
#define MOOON_NAMESPACE_USE using namespace mooon;

#define UTIL_NAMESPACE_BEGIN namespace util {
#define UTIL_NAMESPACE_END                  }
#define UTIL_NAMESPACE_USE using namespace util;

#define SINGLETON_DECLARE(ClassName) \
    private: \
        static ClassName* _pClassName; \
    public: \
        static ClassName* get_singleton();

#define SINGLETON_IMPLEMENT(ClassName) \
    ClassName* ClassName::_pClassName = NULL; \
    ClassName* ClassName::get_singleton() { \
        if (NULL == ClassName::_pClassName) \
            ClassName::_pClassName = new ClassName; \
        return ClassName::_pClassName; \
    }

/** �ص��ӿ� */
#define CALLBACK_INTERFACE

/** ���Ժ� */
#define MOOON_ASSERT assert

/** ������������󳤶� */
#define DOMAIN_NAME_MAX 60
/** ����IP����󳤶� */
#define IP_ADDRESS_MAX sizeof("xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx")

/** �ļ�ȫ������ֽ��� */
#ifdef FILENAME_MAX
#undef FILENAME_MAX
#endif
#define FILENAME_MAX 2048

/** Ŀ¼����ֽ��� */
#ifdef PATH_MAX
#undef PATH_MAX
#endif
#define PATH_MAX 1024

#ifndef	LINE_MAX
#define	LINE_MAX 2048
#endif

/** IO����ͨ�û�������С */
#ifdef IO_BUFFER_MAX
#undef IO_BUFFER_MAX
#endif
#define IO_BUFFER_MAX 4096

/***
  * �벻Ҫʹ��INT_MIN��INT_MAX�Ⱥ꣬��Ӧ��ѡ��ʹ��stl���е�std::numeric_limits�����
  * ͷ�ļ�Ϊ#include <limits>
  */

/***
  * ͨ����Ա���õ��ṹ���׵�ַ
  * @struct_type: �ṹ��������
  * @member_address: ��Ա��ַ
  * @member_name: ��Ա����
  */
#define get_struct_head_address(struct_type, member_name, member_address) \
        ((struct_type *)((char *)(member_address) - offsetof(struct_type, member_name)))

UTIL_NAMESPACE_BEGIN

/***
  * delete�����࣬�����Զ��ͷ�new������ڴ�
  */
template <class ObjectType>
class delete_helper
{
public:
    /***
      * ����һ��delete_helper����
      * @obj: ��Ҫ�Զ�ɾ���Ķ���ָ��
      * @is_array: �Ƿ�Ϊnew����������
      */
    delete_helper(ObjectType*& obj, bool is_array=false)
        :_obj(obj)
        ,_is_array(is_array)
    {
    }

    /** �����У������Զ�����delete��delete []�����ú�ָ�뽫����ΪNULL */
    ~delete_helper()
    {
        if (_is_array)
            delete []_obj;
        else
            delete _obj;

        _obj = NULL; // ����_obj��obj�����ã�����objҲ����Ӱ��
    }

private:
    ObjectType*& _obj;
    bool _is_array;
};

/***
  * malloc�����࣬�����Զ��ͷ�new������ڴ�
  */
template <typename ObjectType>
class free_helper
{
public:
    /***
      * ����һ��free_helper����
      * @obj: ��Ҫ�Զ�ɾ���Ķ���ָ��
      */
    free_helper(ObjectType*& obj)
        :_obj(obj)
    {
    }

    /** �����У������Զ�����free�����ú�ָ�뽫����ΪNULL */
    ~free_helper()
    {
        if (_obj != NULL)
        {
            free(_obj);
            _obj = NULL; // ����_obj��obj�����ã�����objҲ����Ӱ��
        }
    }

private:
    ObjectType*& _obj;
};

/***
  * va_list�����࣬�����Զ�����va_end
  */
class va_list_helper
{
public:
    va_list_helper(va_list& args)
        :_args(args)
    {
    }

    /** �����������Զ�����va_end */
    ~va_list_helper()
    {
        va_end(_args);
    }

private:
    va_list& _args;
};

/***
  * ���������࣬�����Զ��Լ�����������һ�ͼ�һ����
  */
template <typename DataType>
class CountHelper
{
public:
    /** ���캯�����Լ�����m������һ���� */
    CountHelper(DataType& m)
        :_m(m)
    {
        ++m;
    }

    /** �����������Լ�����m���м�һ���� */
    ~CountHelper()
    {
        --_m;
    }

private:
    DataType& _m;
};

/***
  * ����������
  */
typedef enum
{
    handle_error,   /** ������� */
    handle_finish,  /** ����ɹ���� */
    handle_continue /** ����δ��ɣ���Ҫ���� */
}handle_result_t;

UTIL_NAMESPACE_END
#endif // UTIL_CONFIG_H
