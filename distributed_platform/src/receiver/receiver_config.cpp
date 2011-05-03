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
MOOON_NAMESPACE_BEGIN

/** �õ�epoll��С */
uint32_t CReceiverConfig::get_epoll_size() const
{

}

/** �õ�epool�ȴ���ʱ������ */
uint32_t CReceiverConfig::get_epoll_timeout() const;
{

}

/** �õ���ܵĹ����̸߳��� */
uint16_t CReceiverConfig::get_thread_number() const;
{

}

/** �õ����ӳش�С */
uint32_t CReceiverConfig::get_connection_pool_size() const;
{

}

/** ���ӳ�ʱ���� */
uint32_t CReceiverConfig::get_connection_timeout_seconds() const;
{

}

/** �õ��������� */    
const net::ip_port_pair_array_t& CReceiverConfig::get_listen_parameter() const;
{

}

MOOON_NAMESPACE_END
#endif // MOOON_RECEIVER_H
