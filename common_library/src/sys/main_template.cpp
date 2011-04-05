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
#include <signal.h>
#include <unistd.h>
#include <strings.h>
#include <stdexcept>
#include <sys/wait.h>
#include "sys/log.h"
#include "sys/sys_util.h"
#include "util/string_util.h"
#include "sys/main_template.h"
SYS_NAMESPACE_BEGIN

/***
  * �Ƿ��������������źŷ���ʱ�������Ƿ�������:
  * 1) SIGILL
  * 2) SIGFPE
  * 3) SIGBUS
  * 4) SIGABRT
  * 5) SIGSEGV
  */
static bool self_restart();

/***
  * �����̴����߼�
  * @child_pid: �ӽ��̺�
  * @child_exit_code: �ӽ��̵��˳�����
  * @return: ����true������²Ż��������������ӽ��̶��˳�
  */
static bool parent_process(pid_t child_pid, int& child_exit_code);

/***
  * �ӽ��̴����߼�
  */
static void child_process(IMainHelper* main_helper, int argc, char* argv[]);

/***
  * main_template������main�����е��ã�ͨ������һ�д��뼴��:
  * int main(int argc, char* argv[])
  * {
  *     return main_template(argc, argv);
  * }
  */
int main_template(IMainHelper* main_helper, int argc, char* argv[])
{
    // �˳����룬���ӽ��̾���
    int exit_code = 1;

    // ���Ե�PIPE�ź�
    if (SIG_ERR == signal(SIGPIPE, SIG_IGN))
    {
        fprintf(stderr, "Ignored SIGPIPE error: %s\n", sys::CSysUtil::get_last_error_message().c_str());
        return 1;
    }

    while (true)
    {
        pid_t pid = self_restart()? fork(): 0;
        if (-1 == pid)
        {
            // forkʧ��
            fprintf(stderr, "fork error: %s.\n", sys::CSysUtil::get_last_error_message().c_str());
            break;
        }
        else if (0 == pid)
        {
            child_process(main_helper, argc, argv);
        }
        else if (!parent_process(pid, exit_code))
        {
            break;
        }
    }

    return exit_code;
}

bool self_restart()
{
    // �ɻ�������SELF_RESTART�������Ƿ�������
    char* restart = getenv("SELF_RESTART");
    return (restart != NULL)
        && (0 == strcasecmp(restart, "true"));
}

bool parent_process(pid_t child_pid, int& child_exit_code)
{
    // �Ƿ�������
    bool restart = false;

    while (true)
    {
        int status;
        int retval = waitpid(child_pid, &status, 0);
        if (-1 == retval)
        {
            if (EINTR == errno)
            {
                continue;
            }
            else
            {
                fprintf(stderr, "Wait %d error: %s.\n", child_pid, sys::CSysUtil::get_last_error_message().c_str());
            }
        }
        else if (WIFSTOPPED(status))
        {
            child_exit_code = WSTOPSIG(status);
            fprintf(stderr, "Process %d was stopped by signal %d.\n", child_pid, child_exit_code);
        }
        else if (WIFEXITED(status))
        {
            child_exit_code = WEXITSTATUS(status);
            fprintf(stderr, "Process %d was exited with code %d.\n", child_pid, child_exit_code);
        }
        else if (WIFSIGNALED(status))
        {                    
            int signo = WTERMSIG(status);
            fprintf(stderr, "Process %d received signal %d.\n", child_pid, signo);
            child_exit_code = signo;

            if ((SIGILL == signo)   // �Ƿ�ָ��
             || (SIGBUS == signo)   // ���ߴ���
             || (SIGFPE == signo)   // �������
             || (SIGSEGV == signo)  // �δ���
             || (SIGABRT == signo)) // raise
            {
                restart = true;
            }
        }
        else
        {
            fprintf(stderr, "Process %d was exited, but unknown error.\n", child_pid);
        }
    }

    return restart;
}

void child_process(IMainHelper* main_helper, int argc, char* argv[])
{
    sigset_t sigset;
    int exit_code = 0;

    // �յ�SIGUSR1�ź�ʱ�����˳�����
    if (-1 == sigemptyset(&sigset))
    {
        fprintf(stderr, "Initialized signal set error: %s\n", sys::CSysUtil::get_last_error_message().c_str());
        exit(1);
    }
    if (-1 == sigaddset(&sigset, SIGUSR1))
    {
        fprintf(stderr, "Added SIGUSR1 to signal set error: %s\n", sys::CSysUtil::get_last_error_message().c_str());
        exit(1);
    }
    if (-1 == sigprocmask(SIG_BLOCK, &sigset, NULL))
    {
        fprintf(stderr, "Blocked SIGUSR1 error: %s\n", sys::CSysUtil::get_last_error_message().c_str());
        exit(1);
    }

    // ��ʼ��ʧ��
    if (!main_helper->init())
    {
        fprintf(stderr, "Main helper initialized failed.\n");
        exit(1);
    }

    while (true)    
    {
        int signo = -1;
        exit_code = sigwait(&sigset, &signo);        
        if (exit_code != 0)
        {
            fprintf(stderr, "sigwai error: %s.\n", sys::CSysUtil::get_last_error_message().c_str());
            break;
        }
        if (SIGUSR1 == signo)
        {
            break;
        }
    }

    main_helper->fini();
    exit(exit_code);
}

SYS_NAMESPACE_END
