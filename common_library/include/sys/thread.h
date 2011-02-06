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
#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>
#include "sys/event.h"
#include "sys/sys_util.h"
#include "sys/ref_countable.h"
SYS_NAMESPACE_BEGIN

/**
 * �̳߳������
 * ע������: ���ܶ���CThread��ջ����ֻ��Ϊ���Ѷ���
 * ������CThread thread����Ӧ��ΪCThread* thread = new CThread
 */
class CThread: public CRefCountable
{
	friend void* thread_proc(void* thread_param);

private:
    /** �߳�ִ���庯�����������ʵ�ָú������˺����ڵĴ������һ���������߳���ִ�� */
	virtual void run() = 0;
    /** �������߳�֮ǰ�����õĻص��������������false����ᵼ��start����Ҳ����false��
      * ������д�ú��������߳�����֮ǰ���߼��������
      */
    virtual bool before_start() { return true; }

public:
    /** �õ���ǰ�̺߳� */
    static uint32_t get_current_thread_id();

public:
	CThread();
	virtual ~CThread();

    /** ��_stop��Ա����Ϊtrue���߳̿��Ը���_stop״̬�������Ƿ��˳��߳�      
      * @wait_stop: �Ƿ�ȴ��߳̽�����ֻ�е��߳��ǿ�Joinʱ����Ч
      * @exception: ��wait_stopΪtrueʱ���׳���join��ͬ���쳣���������쳣
      */
    virtual void stop(bool wait_stop=true);

    /** �����߳�
      * @detach: �Ƿ��Կɷ���ģʽ�����߳�
      * @exception: ���ʧ�ܣ����׳�CSyscallException�쳣��
      *             �������Ϊbefore_start����false���������Ϊ0
      */
	void start(bool detach=false);

    /** �����߳�ջ��С��Ӧ����start֮ǰ���ã�����������Ч�������before_start���С�
      * @stack_size: ջ��С�ֽ���
      * @exception: ���׳��쳣
      */
    void set_stack_size(size_t stack_size) { _stack_size = stack_size; }
    
    /** �õ��߳�ջ��С�ֽ���
      * @exception: ���ʧ�ܣ����׳�CSyscallException�쳣
      */
    size_t get_stack_size() const;

    /** �õ����̺߳� */
    uint32_t get_thread_id() const { return _thread; }
    
    /** �ȴ��̷߳���
      * @exception: ���ʧ�ܣ����׳�CSyscallException�쳣
      */
    void join();

    /** ���߳�����Ϊ�ɷ���ģ�
      * ��ע��һ�����߳�����Ϊ�ɷ���ģ�������ת��Ϊ��join��
      * @exception: ���ʧ�ܣ����׳�CSyscallException�쳣
      */
    void detach();

    /** �����߳��Ƿ��join
      * @return: ����߳̿�join�򷵻�true�����򷵻�false
      * @exception: ���ʧ�ܣ����׳�CSyscallException�쳣
      */
    bool can_join() const;

    /***
      * ����߳������ڵȴ�״̬������
      */
    void wakeup();

protected: // ��������ʹ��
    /***
      * �ж��߳��Ƿ�Ӧ���˳���Ĭ�Ϸ���_stop��ֵ
      */
    virtual bool is_stop() const;

    /***
      * ���뼶sleep���߳̿��Ե���������˯��״̬�����ҿ���ͨ������do_wakeup���ѣ�
      * ��ע��ֻ���߳̿��Ե��ô˺����������̵߳�����Ч
      */
    void do_millisleep(int milliseconds);

private:
    void do_wakeup(bool stop);

private:    
    CLock _lock;
    CEvent _event;
    volatile bool _stop; /** �Ƿ�ֹͣ�̱߳�ʶ */
    /** �̵߳�ǰ״̬: �ȴ������Ѻ��������� */
    volatile enum { state_sleeping, state_wakeuped, state_running } _current_state;

private:
    pthread_t _thread;
    pthread_attr_t _attr;
    size_t _stack_size;    
};


SYS_NAMESPACE_END
#endif // THREAD_H
