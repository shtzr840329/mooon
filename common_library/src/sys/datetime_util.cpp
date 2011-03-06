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
#include <pthread.h> // localtime_r
#include "util/string_util.h"
#include "sys/datetime_util.h"
SYS_NAMESPACE_BEGIN

bool CDatetimeUtil::is_leap_year(int year)
{
    return ((0 == year%4) && (year%100 != 0)) || (0 == year%400);
}

void CDatetimeUtil::get_current_datetime(char* datetime_buffer, size_t datetime_buffer_size)
{
    struct tm result;
    time_t now = time(NULL);

    localtime_r(&now, &result);
    snprintf(datetime_buffer, datetime_buffer_size
        ,"%04d-%02d-%02d %02d:%02d:%02d"
        ,result.tm_year+1900, result.tm_mon+1, result.tm_mday
        ,result.tm_hour, result.tm_min, result.tm_sec);
}

std::string CDatetimeUtil::get_current_datetime()
{
    char datetime_buffer[sizeof("YYYY-MM-DD HH:SS:MM")];
    get_current_datetime(datetime_buffer, sizeof(datetime_buffer));
    return datetime_buffer;
}

void CDatetimeUtil::get_current_date(char* date_buffer, size_t date_buffer_size)
{
    struct tm result;
    time_t now = time(NULL);

    localtime_r(&now, &result);
    snprintf(date_buffer, date_buffer_size
        ,"%04d-%02d-%02d"
        ,result.tm_year+1900, result.tm_mon+1, result.tm_mday);
}

std::string CDatetimeUtil::get_current_date()
{
    char date_buffer[sizeof("YYYY-MM-DD")];
    get_current_date(date_buffer, sizeof(date_buffer));
    return date_buffer;
}

void CDatetimeUtil::get_current_time(char* time_buffer, size_t time_buffer_size)
{
    struct tm result;
    time_t now = time(NULL);

    localtime_r(&now, &result);
    snprintf(time_buffer, time_buffer_size
        ,"%02d:%02d:%02d"
        ,result.tm_hour, result.tm_min, result.tm_sec);
}

std::string CDatetimeUtil::get_current_time()
{
    char time_buffer[sizeof("HH:SS:MM")];
    get_current_time(time_buffer, sizeof(time_buffer));
    return time_buffer;
}

void CDatetimeUtil::get_current_datetime_struct(struct tm* current_datetime_struct)
{
    time_t now = time(NULL);
    localtime_r(&now, current_datetime_struct);
}

void CDatetimeUtil::to_current_datetime(struct tm* current_datetime_struct, char* datetime_buffer, size_t datetime_buffer_size)
{
    snprintf(datetime_buffer, datetime_buffer_size
        ,"%04d-%02d-%02d %02d:%02d:%02d"
        ,current_datetime_struct->tm_year+1900, current_datetime_struct->tm_mon+1, current_datetime_struct->tm_mday
        ,current_datetime_struct->tm_hour, current_datetime_struct->tm_min, current_datetime_struct->tm_sec);
}

std::string CDatetimeUtil::to_current_datetime(struct tm* current_datetime_struct)
{
    char datetime_buffer[sizeof("YYYY-MM-DD HH:SS:MM")];
    to_current_datetime(current_datetime_struct, datetime_buffer, sizeof(datetime_buffer));
    return datetime_buffer;
}

void CDatetimeUtil::to_current_date(struct tm* current_datetime_struct, char* date_buffer, size_t date_buffer_size)
{
    snprintf(date_buffer, date_buffer_size
        ,"%04d-%02d-%02d"
        ,current_datetime_struct->tm_year+1900, current_datetime_struct->tm_mon+1, current_datetime_struct->tm_mday);
}

std::string CDatetimeUtil::to_current_date(struct tm* current_datetime_struct)
{
    char date_buffer[sizeof("YYYY-MM-DD")];
    to_current_date(current_datetime_struct, date_buffer, sizeof(date_buffer));
    return date_buffer;
}

void CDatetimeUtil::to_current_time(struct tm* current_datetime_struct, char* time_buffer, size_t time_buffer_size)
{
    snprintf(time_buffer, time_buffer_size
        ,"%02d:%02d:%02d"
        ,current_datetime_struct->tm_hour, current_datetime_struct->tm_min, current_datetime_struct->tm_sec);
}

std::string CDatetimeUtil::to_current_time(struct tm* current_datetime_struct)
{
    char time_buffer[sizeof("HH:SS:MM")];
    to_current_date(current_datetime_struct, time_buffer, sizeof(time_buffer));
    return time_buffer;
}

void CDatetimeUtil::to_current_year(struct tm* current_datetime_struct, char* year_buffer, size_t year_buffer_size)
{
    snprintf(year_buffer, year_buffer_size, "%04d", current_datetime_struct->tm_year+1900);
}

std::string CDatetimeUtil::to_current_year(struct tm* current_datetime_struct)
{
    char year_buffer[sizeof("YYYY")];
    to_current_year(current_datetime_struct, year_buffer, sizeof(year_buffer));
    return year_buffer;
}

void CDatetimeUtil::to_current_month(struct tm* current_datetime_struct, char* month_buffer, size_t month_buffer_size)
{
    snprintf(month_buffer, month_buffer_size, "%d", current_datetime_struct->tm_mon+1);
}

std::string CDatetimeUtil::to_current_month(struct tm* current_datetime_struct)
{
    char month_buffer[sizeof("MM")];
    to_current_month(current_datetime_struct, month_buffer, sizeof(month_buffer));
    return month_buffer;
}

void CDatetimeUtil::to_current_day(struct tm* current_datetime_struct, char* day_buffer, size_t day_buffer_size)
{
    snprintf(day_buffer, day_buffer_size, "%d", current_datetime_struct->tm_mday);
}

std::string CDatetimeUtil::to_current_day(struct tm* current_datetime_struct)
{
    char day_buffer[sizeof("DD")];
    to_current_day(current_datetime_struct, day_buffer, sizeof(day_buffer));
    return day_buffer;
}

void CDatetimeUtil::to_current_hour(struct tm* current_datetime_struct, char* hour_buffer, size_t hour_buffer_size)
{
    snprintf(hour_buffer, hour_buffer_size, "%d", current_datetime_struct->tm_hour);
}

std::string CDatetimeUtil::to_current_hour(struct tm* current_datetime_struct)
{
    char hour_buffer[sizeof("HH")];
    to_current_hour(current_datetime_struct, hour_buffer, sizeof(hour_buffer));
    return hour_buffer;
}

void CDatetimeUtil::to_current_minite(struct tm* current_datetime_struct, char* minite_buffer, size_t minite_buffer_size)
{
    snprintf(minite_buffer, minite_buffer_size, "%d", current_datetime_struct->tm_min);
}

std::string CDatetimeUtil::to_current_minite(struct tm* current_datetime_struct)
{
    char minite_buffer[sizeof("MM")];
    to_current_minite(current_datetime_struct, minite_buffer, sizeof(minite_buffer));
    return minite_buffer;
}

void CDatetimeUtil::to_current_second(struct tm* current_datetime_struct, char* second_buffer, size_t second_buffer_size)
{
    snprintf(second_buffer, second_buffer_size, "%d", current_datetime_struct->tm_sec);
}

std::string CDatetimeUtil::to_current_second(struct tm* current_datetime_struct)
{
    char second_buffer[sizeof("SS")];
    to_current_second(current_datetime_struct, second_buffer, sizeof(second_buffer));
    return second_buffer;
}

bool CDatetimeUtil::datetime_struct_from_string(const char* str, struct tm* datetime_struct)
{
    const char* tmp_str = str;

#ifdef _XOPEN_SOURCE
    return strptime(tmp_str, "%Y-%m-%d %H:%M:%S", datetime_struct) != NULL;
#else
    size_t str_len = strlen(tmp_str);
    if (str_len != sizeof("YYYY-MM-DD HH:MM:SS")-1) return false;
    if ((tmp_str[4] != '-')
     || (tmp_str[7] != '-')
     || (tmp_str[10] != ' ')
     || (tmp_str[13] != ':')
     || (tmp_str[16] != ':'))
        return false;

    using namespace util;
    if (!CStringUtil::string2int32(tmp_str, datetime_struct->tm_year, sizeof("YYYY")-1, false)) return false;
    if ((datetime_struct->tm_year > 3000) || (datetime_struct->tm_year < 1900)) return false;

    tmp_str += sizeof("YYYY");
    if (!CStringUtil::string2int32(tmp_str, datetime_struct->tm_mon, sizeof("MM")-1, true)) return false;
    if ((datetime_struct->tm_mon > 12) || (datetime_struct->tm_mon < 1)) return false;

    tmp_str += sizeof("MM");
    if (!CStringUtil::string2int32(tmp_str, datetime_struct->tm_mday, sizeof("DD")-1, true)) return false;
    if (datetime_struct->tm_mday < 1) return false;

    // 闰年二月可以有29天
    if ((CDatetimeUtil::is_leap_year(datetime_struct->tm_year)) && (2 == datetime_struct->tm_mon) && (datetime_struct->tm_mday > 29))
        return false;
    else if (datetime_struct->tm_mday > 28)
        return false;

    tmp_str += sizeof("DD");
    if (!CStringUtil::string2int32(tmp_str, datetime_struct->tm_hour, sizeof("HH")-1, true)) return false;
    if ((datetime_struct->tm_hour > 24) || (datetime_struct->tm_hour < 0)) return false;

    tmp_str += sizeof("HH");
    if (!CStringUtil::string2int32(tmp_str, datetime_struct->tm_min, sizeof("MM")-1, true)) return false;
    if ((datetime_struct->tm_min > 60) || (datetime_struct->tm_min < 0)) return false;

    tmp_str += sizeof("MM");
    if (!CStringUtil::string2int32(tmp_str, datetime_struct->tm_sec, sizeof("SS")-1, true)) return false;
    if ((datetime_struct->tm_sec > 60) || (datetime_struct->tm_sec < 0)) return false;

    datetime_struct->tm_isdst = 0;
    datetime_struct->tm_wday  = 0;
    datetime_struct->tm_yday  = 0;

    // 计算到了一年中的第几天
    for (int i=1; i<=datetime_struct->tm_mon; ++i)
    {
        if (i == datetime_struct->tm_mon)
        {
            // 刚好是这个月
            datetime_struct->tm_yday += datetime_struct->tm_mday;
        }
        else
        {
            // 1,3,5,7,8,10,12
            if ((1 == i) || (3 == i) || (5 == i) || (7 == i) || (8 == i) || (10 == i) || (12 == i))
            {
                datetime_struct->tm_yday += 31;
            }
            else if (2 == i)
            {
                if (CDatetimeUtil::is_leap_year(datetime_struct->tm_year))
                    datetime_struct->tm_yday += 29;
                else
                    datetime_struct->tm_yday += 28;
            }
            else
            {
                datetime_struct->tm_yday += 30;
            }
        }
    }

    // 月基数
    static int leap_month_base[] = { -1, 0, 3, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6 };
    static int common_month_base[] = { -1, 0, 3, 3, 6, 1, 4, 0, 3, 5, 0, 3, 5 };

    int year_base;
    int *month_base;
    if (CDatetimeUtil::is_leap_year(datetime_struct->tm_year))
    {
         year_base = 2;
         month_base = leap_month_base;
    }
    else
    {
         year_base = 1;
         month_base = common_month_base;
    }

    // 计算星期几
    datetime_struct->tm_wday = (datetime_struct->tm_year
                             +  datetime_struct->tm_year / 4
                             +  datetime_struct->tm_year / 400
                             -  datetime_struct->tm_year / 100
                             -  year_base
                             +  month_base[datetime_struct->tm_mon]
                             +  datetime_struct->tm_mday) / 7;

    // 年月处理
    datetime_struct->tm_mon -= 1;
    datetime_struct->tm_year -= 1900;
    return true;
#endif // _XOPEN_SOURCE
}

bool CDatetimeUtil::datetime_struct_from_string(const char* str, time_t* datetime)
{
    struct tm datetime_struct;
    if (!datetime_struct_from_string(str, &datetime_struct)) return false;

    *datetime = mktime(&datetime_struct);
    return true;
}

SYS_NAMESPACE_END
