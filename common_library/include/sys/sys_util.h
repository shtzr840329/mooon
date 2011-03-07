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
#ifndef SYS_UTIL_H
#define SYS_UTIL_H
#include "sys/syscall_exception.h"
SYS_NAMESPACE_BEGIN

/***
  * 与系统调用有关的工具类函数实现
  */
class CSysUtil
{
public:
    /** 线程安全的毫秒级sleep函数
      * @millisecond: 需要sleep的毫秒数
      */
    static void millisleep(uint32_t millisecond);

    /** 得到指定系统调用错误码的字符串错误信息
      * @errcode: 系统调用错误码
      * @return: 系统调用错误信息
      */
    static std::string get_error_message(int errcode);

    /** 得到当前进程所属可执行文件所在的绝对路径，结尾符不含反斜杠 */
    static std::string get_program_path();
    
    /** 得到与指定fd相对应的文件名，包括路径部分
      * @fd: 文件描述符
      * @return: 文件名，包括路径部分，如果失败则返回空字符串
      */
	static std::string get_filename(int fd);

    /** 得到一个目录的绝对路径，路径中不会包含../和./等，是一个完整的原始路径
      * @directory: 目录的相对或绝对路径
      * @return: 返回目录的绝对路径，如果出错则返回空字符串
      */
    static std::string get_full_directory(const char* directory);

    /** 得到CPU核个数
      * @return: 如果成功，返回大于0的CPU核个数，否则返回0
      */
	static uint16_t get_cpu_number();    

    /** 得到当前调用栈
      * 注意事项: 编译源代码时带上-rdynamic和-g选项，否则可能看到的是函数地址，而不是函数符号名称
      * @call_stack: 存储调用栈
      * @return: 成功返回true，否则返回false
      */
    static bool get_backtrace(std::string& call_stack);

    /** 得到指定目录字节数大小，非线程安全函数，同一时刻只能被一个线程调用
      * @dirpath: 目录路径
      * @return: 目录字节数大小
      */
    static off_t du(const char* dirpath);

    /** 得到内存页大小 */
    static int get_page_size();

    /** 得到一个进程可持有的最多文件(包括套接字等)句柄数 */
    static int get_fd_max();

    /***
      * 递归的创建目录
      * @dirpath: 需要创建的目录      
      * @permissions: 目录权限，取值可以为下列的任意组合:
      *                    S_IRWXU, S_IRUSR, S_IWUSR, S_IXUSR
      *                    S_IRWXG, S_IRGRP, S_IWGRP, S_IXGRP
      *                    S_IRWXO, S_IROTH, S_IWOTH, S_IXOTH
      * @exception: 出错则抛出CSyscallException
      */
    static void create_directory(const char* dirpath, int permissions=DIRECTORY_DEFAULT_PERM);

    /***
      * 递归的创建目录
      * @dirpath: 需要创建的目录      
      * @permissions: 目录权限
      * @exception: 出错则抛出CSyscallException
      */
    static void create_directory_recursive(const char* dirpath, int permissions=DIRECTORY_DEFAULT_PERM);

    /** 下列is_xxx函数如果发生错误，则抛出CSyscallException异常 */
    static bool is_file(int fd);                 /** 判断指定fd对应的是否为文件 */
    static bool is_file(const char* path);       /** 判断指定Path是否为一个文件 */
    static bool is_link(int fd);                 /** 判断指定fd对应的是否为软链接 */
    static bool is_link(const char* path);       /** 判断指定Path是否为一个软链接 */
    static bool is_directory(int fd);            /** 判断指定fd对应的是否为目录 */
    static bool is_directory(const char* path);  /** 判断指定Path是否为一个目录 */
    
    /***
      * 是否允许当前进程生成coredump文件
      * @enable: 如果为true，则允许当前进程生成coredump文件，否则禁止
      * @core_file_size: 允许生成的coredump文件大小，如果取值小于0，则表示不限制文件大小
      * @exception: 如果调用出错，则抛出CSyscallException异常
      */
    static void enable_core_dump(bool enabled=true, int core_file_size=-1);

    /** 得到当前进程名，包括路径部分 */
    static const char* get_program_name();

    /** 得到当前进程的短名字，即纯文件名 */
    static const char* get_program_short_name();
};

SYS_NAMESPACE_END
#endif // SYS_UTIL_H
