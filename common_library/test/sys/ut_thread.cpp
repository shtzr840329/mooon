﻿#include <sys/thread.h>
#include <sys/sys_util.h>

// 所有非线程池线程都应当是CThread的子类
// Exam线程CDemoThread运行后，每隔1秒往标准输出打印一行“continue...”
class CExamThread: public sys::CThread
{
private:
	virtual void run();
	virtual bool before_start();
};

bool CExamThread::before_start()
{
    // start的返回值和before_start的相同，一般用于线程启动前的初始化
	return true;
}

// 线程每秒钟往标准输出打印一次continue...
void CExamThread::run()
{
    // stop将使得is_stop返回false
	while (!is_stop())
	{
	    // 睡眠1秒钟
        // do_millisleep是由CThread提供给子类睡眠使用的，
        // 可通过调用wakeup将睡眠中断
	    do_millisleep(1000);
	    printf("continue ...\n");
	}
}

int main()
{
	// 创建并启动线程
	CExamThread* thread = new CExamThread;
	thread->inc_refcount();
	try
	{
	    thread->start();
	}
	catch (sys::CSyscallException& ex)
	{
	    printf("Start thread error: %s\n"
	         , sys::CSysUtil::get_error_message(ex.get_errcode()).c_str());
	    thread->dec_refcount();
	    exit(1);
	}
	
	// 主线程睡眠10秒钟
	sys::CSysUtil::millisleep(10000);
	
	thread->stop(); // 停止并待线程退出
	thread->dec_refcount(); // 记得增加了引用计数，就需要在使用完后，相应的减引用计数
	
	return 0;
}
