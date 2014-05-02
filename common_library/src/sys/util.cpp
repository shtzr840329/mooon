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
#include <ftw.h> // ftw
#include <time.h>
#include <dirent.h>
#include <execinfo.h> // backtrace和backtrace_symbols函数
#include <sys/time.h>
#include <features.h> // feature_test_macros
#include <sys/prctl.h> // prctl
#include <sys/types.h>
#include <sys/resource.h>
#include <util/token_list.h>
#include <util/string_util.h>
#include "sys/util.h"
#include "sys/close_helper.h"

#ifndef PR_SET_NAME
#define PR_SET_NAME 15
#endif
 
#ifndef PR_GET_NAME
#define PR_GET_NAME 16
#endif

////////////////////////////////////////////////////////////////////////////////
SYS_NAMESPACE_BEGIN

// 和set_program_title相关
static char *g_arg_start = NULL;
static char *g_arg_end   = NULL;
static char *g_env_start = NULL;

void CUtil::millisleep(uint32_t millisecond)
{
    struct timespec ts = { millisecond / 1000, (millisecond % 1000) * 1000000 };
    while ((-1 == nanosleep(&ts, &ts)) && (EINTR == errno));
}

std::string CUtil::get_error_message(int errcode)
{
    return Error::to_string(errcode);
}

std::string CUtil::get_last_error_message()
{
    return Error::to_string();
}

int CUtil::get_last_error_code()
{
    return Error::code();
}

int CUtil::get_current_process_id()
{
    return getpid();
}

std::string CUtil::get_program_path()
{
    char buf[1024];

    buf[0] = '\0';
    int retval = readlink("/proc/self/exe", buf, sizeof(buf)-1);
    if (retval > 0)
    {
        buf[retval] = '\0';

#if 0 // 保留这段废代码，以牢记deleted的存在，但由于这里只取路径部分，所以不关心它的存在        
        if (!strcmp(buf+retval-10," (deleted)"))
            buf[retval-10] = '\0';
#else

        // 去掉文件名部分
        char* end = strrchr(buf, '/');
        if (NULL == end)
            buf[0] = 0;
        else
            *end = '\0';
#endif
    }

    return buf;
}

std::string CUtil::get_filename(int fd)
{
	char path[PATH_MAX];
	char filename[FILENAME_MAX] = {'\0'};
	
	snprintf(path, sizeof(path), "/proc/%d/fd/%d", getpid(), fd);
	if (-1 == readlink(path, filename, sizeof(filename))) filename[0] = '\0';
    
	return filename;
}

// 库函数：char *realpath(const char *path, char *resolved_path);
//         char *canonicalize_file_name(const char *path);
std::string CUtil::get_full_directory(const char* directory)
{
    std::string full_directory;
    DIR* dir = opendir(directory);
    if (dir != NULL)
    {
        int fd = dirfd(dir);
        if (fd != -1)
            full_directory = get_filename(fd);

        closedir(dir);
    }
 
    return full_directory;
}

// 相关函数：
// get_nprocs()，声明在sys/sysinfo.h
// sysconf(_SC_NPROCESSORS_CONF)
// sysconf(_SC_NPROCESSORS_ONLN)
uint16_t CUtil::get_cpu_number()
{
	FILE* fp = fopen("/proc/cpuinfo", "r");
	if (NULL == fp) return 1;
	
	char line[LINE_MAX];
	uint16_t cpu_number = 0;
    sys::CloseHelper<FILE*> ch(fp);

	while (fgets(line, sizeof(line)-1, fp))
	{
		char* name = line;
		char* value = strchr(line, ':');
		
		if (NULL == value)
			continue;

		*value++ = 0;		
		if (0 == strncmp("processor", name, sizeof("processor")-1))
		{
			 if (!util::CStringUtil::string2uint16(value, cpu_number))
             {
                 return 0;
             }
		}
	}

	return (cpu_number+1);
}

bool CUtil::get_backtrace(std::string& call_stack)
{
    const int frame_number_max = 20;       // 最大帧层数
    void* address_array[frame_number_max]; // 帧地址数组

    // real_frame_number的值不会超过frame_number_max，如果它等于frame_number_max，则表示顶层帧被截断了
    int real_frame_number = backtrace(address_array, frame_number_max);

    char** symbols_strings = backtrace_symbols(address_array, real_frame_number);
    if (NULL == symbols_strings)
    {
        return false;
    }
    else if (real_frame_number < 2) 
    {
        free(symbols_strings);
        return false;
    }

    call_stack = symbols_strings[1]; // symbols_strings[0]为get_backtrace自己，不显示
    for (int i=2; i<real_frame_number; ++i)
    {
        call_stack += std::string("\n") + symbols_strings[i];
    }

    free(symbols_strings);
    return true;
}

static off_t dirsize; // 目录大小
int _du_fn(const char *fpath, const struct stat *sb, int typeflag)
{   
    if (FTW_F == typeflag)
        dirsize += sb->st_size;

    return 0;
}

off_t CUtil::du(const char* dirpath)
{
    dirsize = 0;
    if (ftw(dirpath, _du_fn, 0) != 0) return -1;

    return dirsize;
}

int CUtil::get_page_size()
{
    // sysconf(_SC_PAGE_SIZE);
    // sysconf(_SC_PAGESIZE);
    return getpagesize();
}

int CUtil::get_fd_max()
{
    // sysconf(_SC_OPEN_MAX);
    return getdtablesize();
}

void CUtil::create_directory(const char* dirpath, mode_t permissions)
{
    if (-1 == mkdir(dirpath, permissions))
        if (errno != EEXIST)
            throw sys::CSyscallException(errno, __FILE__, __LINE__);
}

void CUtil::create_directory_recursive(const char* dirpath, mode_t permissions)
{
    char* slash;
    char* pathname = strdupa(dirpath); // _GNU_SOURCE
    char* pathname_p = pathname;
    
    // 过滤掉头部的斜杠
    while ('/' == *pathname_p) ++pathname_p;

    for (;;)
    {
        slash = strchr(pathname_p, '/');
        if (NULL == slash) // 叶子目录
        {
            if (0 == mkdir(pathname, permissions)) break;            
            if (EEXIST == errno) break;
            
            throw sys::CSyscallException(errno, __FILE__, __LINE__);
        }

        *slash = '\0';
        if ((-1 == mkdir(pathname, permissions)) && (errno != EEXIST))        
            throw sys::CSyscallException(errno, __FILE__, __LINE__);
        
        *slash++ = '/';
        while ('/' == *slash) ++slash; // 过滤掉相连的斜杠
        pathname_p = slash;
    }
}

void CUtil::create_directory_byfilepath(const char* filepath, mode_t permissions)
{
    std::string dirpath = util::CStringUtil::extract_dirpath(filepath);
    create_directory_recursive(dirpath.c_str(),  permissions);
}

bool CUtil::is_file(int fd)
{
    struct stat buf;
    if (-1 == fstat(fd, &buf))
        throw sys::CSyscallException(errno, __FILE__, __LINE__);

    return S_ISREG(buf.st_mode);
}

bool CUtil::is_file(const char* path)
{
    struct stat buf;
    if (-1 == stat(path, &buf))
        throw sys::CSyscallException(errno, __FILE__, __LINE__);

    return S_ISREG(buf.st_mode);
}

bool CUtil::is_link(int fd)
{
    struct stat buf;
    if (-1 == fstat(fd, &buf))
        throw sys::CSyscallException(errno, __FILE__, __LINE__);

    return S_ISLNK(buf.st_mode);
}

bool CUtil::is_link(const char* path)
{
    struct stat buf;
    if (-1 == stat(path, &buf))
        throw sys::CSyscallException(errno, __FILE__, __LINE__);

    return S_ISLNK(buf.st_mode);
}

bool CUtil::is_directory(int fd)
{
    struct stat buf;
    if (-1 == fstat(fd, &buf))
        throw sys::CSyscallException(errno, __FILE__, __LINE__);

    return S_ISDIR(buf.st_mode);
}

bool CUtil::is_directory(const char* path)
{
    struct stat buf;
    if (-1 == stat(path, &buf))
        throw sys::CSyscallException(errno, __FILE__, __LINE__);

    return S_ISDIR(buf.st_mode);
}

void CUtil::enable_core_dump(bool enabled, int core_file_size)
{    
    if (enabled)
    {
        struct rlimit rlim;
        rlim.rlim_cur = (core_file_size < 0)? RLIM_INFINITY: core_file_size;
        rlim.rlim_max = rlim.rlim_cur;

        if (-1 == setrlimit(RLIMIT_CORE, &rlim))
            throw sys::CSyscallException(errno, __FILE__, __LINE__);
    }       
    
    if (-1 == prctl(PR_SET_DUMPABLE, enabled? 1: 0))
        throw sys::CSyscallException(errno, __FILE__, __LINE__);
}

const char* CUtil::get_program_name()
{
    return program_invocation_name;
}

const char* CUtil::get_program_short_name()
{
    return program_invocation_short_name;
}

void CUtil::set_process_name(const std::string& new_name)
{
    if (!new_name.empty())
    {
        if (-1 == prctl(PR_SET_NAME, new_name.c_str()))
            throw sys::CSyscallException(errno, __FILE__, __LINE__, "prctl name");
    }
}

void CUtil::set_process_name(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    util::VaListHelper vlh(args);

    char name[NAME_MAX];
    vsnprintf(name, sizeof(name), format, args);
    set_process_name(std::string(name));
}

// 参考：http://www.stamhe.com/?p=416
void CUtil::init_process_title(int argc, char *argv[])
{
    g_arg_start = argv[0];
    g_arg_end = argv[argc-1] + strlen(argv[argc-1]) + 1;
    g_env_start = environ[0];
}

void CUtil::set_process_title(const std::string &new_title)
{
    if (!new_title.empty())
    {
        size_t new_title_len = new_title.length();

        // 新的title比老的长
        if ((static_cast<size_t>(g_arg_end-g_arg_start) < new_title_len)
         && (g_env_start == g_arg_end))
        {
            char *env_end = g_env_start;
            for (int i=0; environ[i]; ++i)
            {
                if (env_end != environ[i])
                {
                    break;
                }

                env_end = environ[i] + strlen(environ[i]) + 1;
                environ[i] = strdup(environ[i]);
            }

            g_arg_end = env_end;
            g_env_start = NULL;
        }

        size_t len = g_arg_end - g_arg_start;
        if (len == new_title_len)
        {
             strcpy(g_arg_start, new_title.c_str());
        }
        else if(new_title_len < len)
        {
            strcpy(g_arg_start, new_title.c_str());
            memset(g_arg_start+new_title_len, 0, len-new_title_len);

            // 当新的title比原title短时，
            // 填充argv[0]字段时，改为填充argv[0]区的后段，前段填充0
            memset(g_arg_start, 0, len);
            strcpy(g_arg_start+(len - new_title_len), new_title.c_str());
        }
        else
        {
            *(char *)mempcpy(g_arg_start, new_title.c_str(), len-1) = '\0';
        }

        if (g_env_start != NULL)
        {
            char *p = strchr(g_arg_start, ' ');
            if (p != NULL)
            {
                *p = '\0';
            }
        }
    }
}

void CUtil::set_process_title(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    util::VaListHelper vlh(args);

    char title[PATH_MAX];
    vsnprintf(title, sizeof(title), format, args);
    set_process_title(std::string(title));
}

void CUtil::common_pipe_read(int fd, char** buffer, int32_t* buffer_size)
{
	int ret = 0;
	int32_t size = 0;

	// 第一个while循环读取大小
	while (true)
	{
		ret = read(fd, &size, sizeof(size));
		if ((-1 == ret) && (EINTR == errno))
			continue;
		if (-1 == ret)
			throw sys::CSyscallException(errno, __FILE__, __LINE__, "common_pipe_read size");

		break;
	}

	*buffer_size = size;
	*buffer = new char[size];
	char* bufferp =  *buffer;

	// 第二个while循环根据大小读取内容
	while (size > 0)
	{
		ret = read(fd, bufferp, size);
		if ((0 == ret) || (ret == size))
			break;
		if ((-1 == ret) && (EINTR == errno))
			continue;
		if (-1 == ret)
		{
			delete *buffer;
			throw sys::CSyscallException(errno, __FILE__, __LINE__, "common_pipe_read buffer");
		}

		bufferp += ret;
		size -= ret;
	}
}

void CUtil::common_pipe_write(int fd, const char* buffer, int32_t buffer_size)
{
	int ret = 0;
	int32_t size = buffer_size;

	// 第一个while循环写入大小
	while (true)
	{
		ret = write(fd, &size, sizeof(size));
		if ((-1 == ret) && (EINTR == errno))
			continue;
		if (-1 == ret)
			throw sys::CSyscallException(errno, __FILE__, __LINE__, "common_pipe_write size");

		break;
	}

	const char* bufferp = buffer;

	// 第二个while循环根据大小写入内容
	while (size > 0)
	{
		ret = write(fd, bufferp, size);
		if ((-1 == ret) && (EINTR == errno))
			continue;
		if (-1 == ret)
			throw sys::CSyscallException(errno, __FILE__, __LINE__, "common_pipe_write buffer");

		size -= ret;
		bufferp += ret;
	}
}

SYS_NAMESPACE_END
