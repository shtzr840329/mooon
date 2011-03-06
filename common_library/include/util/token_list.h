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
#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H
#include <list>
#include "util/util_config.h"
UTIL_NAMESPACE_BEGIN

/***
  * ���ַ�����ָ���ķָ��ַ���������һ��������
  */
class CTokenList
{
public:
    /** �洢Token������ */
    typedef std::list<std::string> TTokenList;

    /***
      * ��ָ�����ַ�����Ϊ�ָ������������ַ��и�Token������һ��Token������
      * @token_list: �洢Token������
      * @source: ���������ַ���
      * @sep: Token�ָ���
      */
    static void parse(TTokenList& token_list, const std::string& source, const std::string& sep);
};

UTIL_NAMESPACE_END
#endif // TOKEN_LIST_H
