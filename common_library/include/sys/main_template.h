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
 * ��Ҫ����:
 * 1) �Զ��������̹��ܣ�Ҫ�󻷾�����SELF_RESTART���ڣ���ֵΪtrue������true�����ִ�Сд
 * 2) ��ʼ��init�ͷ���ʼ��fini�������Զ����ã�ע�������ʼ��init���ɹ����򲻻���÷���ʼ��fini
 * 3) �յ�SIGUSR1�ź��˳����̣��˳�֮ǰ�����fini
 * ע�⣬ֻ֧�������źŷ���ʱ���Զ�����:
 * SIGILL��SIGBUS��SIGFPE��SIGSEGV��SIGABRT
 */
#ifndef MOOON_SYS_MAIN_TEMPLATE_H
#define MOOON_SYS_MAIN_TEMPLATE_H
#include <signal.h>
#include "sys/log.h"
SYS_NAMESPACE_BEGIN

/***
  * main���������ӿڣ����ڰ����Զ���ĳ�ʼ��
  */
class IMainHelper
{
public:
    virtual ~IMainHelper() {}

    /***
      * ��ʼ�������̿�ʼʱ����
	  * ����init����true����false���ڽ����˳�֮ǰһ�����ǵ���fini
	  * ���init����false�������fini�����˳����̣��˳���Ϊ1
      */
    virtual bool init(int argc, char* argv[]) = 0;

    /***
      * �������������˳�֮ǰ����
      */
    virtual void fini() = 0;

    /***
      * �õ���־��
      * @return �������NULL��ֱ���������Ļ
      */
    virtual ILogger* get_logger() const { return NULL; }

    /***
      * �õ��˳��źţ����յ����ź�ʱ�������Զ��˳����˳�֮ǰ����fini
      * @return �������0����ʾ�����κδ��������յ�ָ���ź�ʱ�˳�
      */
    virtual int get_exit_signal() const { return 0; }

    /***
      * �Ƿ����PIPE�ź�
      * @return �������true����ʾ����PIPE�źţ����򲻺���
      */
    virtual bool ignore_pipe_signal() const { return true; }

    /***
      * �õ������������ܵĻ���������
      * @return ������ؿգ������ո����ʾ���øù���
      */
    virtual std::string get_restart_env_name() const { return "SELF_RESTART"; }
    
    /***
      * �õ��������΢������Ĭ��Ϊ1��
      */
    virtual uint32_t get_restart_milliseconds() const { return 1000; }
};

/***
  * ͨ��main������ģ�壬
  * main_template������main�����е��ã�ͨ������һ�д��뼴��:
  * int main(int argc, char* argv[])
  * {
  *     IMainHelper* main_helper = new CMainHelper();
  *     return main_template(main_helper, argc, argv);
  * }
  */
extern int main_template(IMainHelper* main_helper, int argc, char* argv[]);

SYS_NAMESPACE_END
#endif // MOOON_SYS_MAIN_TEMPLATE_H
