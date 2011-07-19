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
 * Author: JianYi, eyjian@qq.com
 */
#ifndef MOOON_SERVER_THREAD_FOLLOWER_H
#define MOOON_SERVER_THREAD_FOLLOWER_H
#include <util/config.h>
MOOON_NAMESPACE_BEGIN
namespace server {

/***
  * 线程伙计
  */
class IThreadFollower
{
public:
    virtual ~IThreadFollower() {}

    /***
      * 线程run之前被调用
      * @return 如果返回true，则会进入run过程，否则线程绕过run而退出
      */
    virtual bool before_run() { return true; }

    /***
      * 线程run之后被调用
      */
    virtual void after_run() {}
};

} // namespace server
MOOON_NAMESPACE_END
#endif // MOOON_SERVER_THREAD_FOLLOWER_H
