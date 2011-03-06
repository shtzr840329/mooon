﻿/**
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
#ifndef DATETIME_UTIL_H
#define DATETIME_UTIL_H
#include <time.h>
#include "sys/sys_util.h"
SYS_NAMESPACE_BEGIN

/** 日期时间工具类
  */
class CDatetimeUtil
{
public:
    /** 判断指定年份是否为闰年 */
    static bool is_leap_year(int year);
    
    /** 得到当前日期和时间，返回格式为: YYYY-MM-DD HH:SS:MM
      * @datetime_buffer: 用来存储当前日期和时间的缓冲区
      * @datetime_buffer_size: datetime_buffer的字节大小，不应当于小“YYYY-MM-DD HH:SS:MM”
      */
    static void get_current_datetime(char* datetime_buffer, size_t datetime_buffer_size);
    static std::string get_current_datetime();    

    /** 得到当前日期，返回格式为: YYYY-MM-DD
      * @date_buffer: 用来存储当前日期的缓冲区
      * @date_buffer_size: date_buffer的字节大小，不应当于小“YYYY-MM-DD”
      */
    static void get_current_date(char* date_buffer, size_t date_buffer_size);
    static std::string get_current_date();

    /** 得到当前时间，返回格式为: HH:SS:MM
      * @time_buffer: 用来存储当前时间的缓冲区
      * @time_buffer_size: time_buffer的字节大小，不应当于小“HH:SS:MM”
      */
    static void get_current_time(char* time_buffer, size_t time_buffer_size);
    static std::string get_current_time();

    /** 得到当前日期和时间结构
      * @current_datetime_struct: 指向当前日期和时间结构的指针
      */
    static void get_current_datetime_struct(struct tm* current_datetime_struct);

    /** 日期和时间 */
    static void to_current_datetime(struct tm* current_datetime_struct, char* datetime_buffer, size_t datetime_buffer_size);
    static std::string to_current_datetime(struct tm* current_datetime_struct);

    /** 仅日期 */
    static void to_current_date(struct tm* current_datetime_struct, char* date_buffer, size_t date_buffer_size);
    static std::string to_current_date(struct tm* current_datetime_struct);

    /** 仅时间 */
    static void to_current_time(struct tm* current_datetime_struct, char* time_buffer, size_t time_buffer_size);
    static std::string to_current_time(struct tm* current_datetime_struct);

    /** 仅年份 */
    static void to_current_year(struct tm* current_datetime_struct, char* year_buffer, size_t year_buffer_size);
    static std::string to_current_year(struct tm* current_datetime_struct);

    /** 仅月份 */
    static void to_current_month(struct tm* current_datetime_struct, char* month_buffer, size_t month_buffer_size);
    static std::string to_current_month(struct tm* current_datetime_struct);

    /** 仅天 */
    static void to_current_day(struct tm* current_datetime_struct, char* day_buffer, size_t day_buffer_size);
    static std::string to_current_day(struct tm* current_datetime_struct);

    /** 仅小时 */
    static void to_current_hour(struct tm* current_datetime_struct, char* hour_buffer, size_t hour_buffer_size);
    static std::string to_current_hour(struct tm* current_datetime_struct);

    /** 仅分钟 */
    static void to_current_minite(struct tm* current_datetime_struct, char* minite_buffer, size_t minite_buffer_size);
    static std::string to_current_minite(struct tm* current_datetime_struct);

    /** 仅秒钟 */
    static void to_current_second(struct tm* current_datetime_struct, char* second_buffer, size_t second_buffer_size);
    static std::string to_current_second(struct tm* current_datetime_struct);

    /***
      * 将一个字符串转换成日期时间格式
      * @str: 符合“YYYY-MM-DD HH:MM:SS”格式的日期时间
      * @datetime_struct: 存储转换后的日期时间
      * @return: 转换成功返回true，否则返回false
      */
    static bool datetime_struct_from_string(const char* str, struct tm* datetime_struct);
    static bool datetime_struct_from_string(const char* str, time_t* datetime);
};

SYS_NAMESPACE_END
#endif // DATETIME_UTIL_H
