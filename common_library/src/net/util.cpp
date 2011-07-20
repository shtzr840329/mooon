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
#include <netdb.h>
#include <net/if.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>
#include <sys/socket.h>
#include "net/util.h"
#include "sys/close_helper.h"
#include "sys/syscall_exception.h"
NET_NAMESPACE_BEGIN

bool CUtil::is_little_endian()
{    
#if defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN)
#   if (__BYTE_ORDER == __LITTLE_ENDIAN)
        return true;
#   else
        return false;
#   endif
#else
    union
    {
        uint16_t a;
        uint8_t  b[2];
    }x;

    // 小字节序：数据的低字节放在低地址处
    // 不管何平台，编译器总是保证：&b[0] < &b[1]    

    x.a = 0x0102; // 01为高字节，02为低字节
    return (x.b[0] == 0x02) && (x.b[1] == 0x01);
#endif // LITTLE_ENDIAN
}

void CUtil::reverse_bytes(const void* source, void* result, size_t length)
{
    uint8_t* source_begin = (uint8_t*)source;
    uint8_t* result_end = ((uint8_t*)result) + length;

    for (size_t i=0; i<length; ++i)
        *(--result_end) = source_begin[i];
}

void CUtil::host2net(const void* source, void* result, size_t length)
{
    /* 只有小字节序才需要转换，大字节序和网络字节序是一致的 */
    if (is_little_endian())    
        CUtil::reverse_bytes(source, result, length);    
}

void CUtil::net2host(const void* source, void* result, size_t length)
{
    CUtil::host2net(source, result, length);
}

bool CUtil::is_host_name(const char* str)
{
    return true;
}

bool CUtil::is_valid_ip(const char* str)
{
    return is_valid_ipv4(str) || is_valid_ipv6(str);
}

bool CUtil::is_valid_ipv4(const char* str)
{
    //127.127.127.127
    if ((NULL == str) || (0 == str[0]) || ('0' == str[0])) return false;
    if (0 == strcmp(str, "*")) return true;
    
    // 排除255.255.255.255
    if (0 == strcmp(str, "255.255.255.255")) return false;
    
    int dot = 0; // .个数
    const char* strp = str;
    while (*strp)
    {
        if ('.' == *strp)
        {
            ++dot;
        }
        else
        {
            // 非数字也不行
            if ((*strp < '0') || (*strp > '9'))
                return false;
        }
        
        ++strp;
    }
    
    // 排除长度    
    // 127.127.127.127
    if (strp-str >= 16)
        return false;
    
    // .的个数必须为3
    return (3 == dot);
}

bool CUtil::is_valid_ipv6(const char* str)
{    
    char* colon = strchr(str, ':');
    if (NULL == colon) return false;
    return strchr(colon, ':') != NULL;
}

bool CUtil::get_ip_address(const char* hostname, string_ip_array_t& ip_array, std::string& errinfo)
{    
    struct addrinfo hints;
    struct addrinfo *result = NULL;
    char ip[IP_ADDRESS_MAX];

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family    = AF_UNSPEC;   /* Allow IPv4 or IPv6 */
    hints.ai_socktype  = SOCK_STREAM; /* Data stream socket */
    hints.ai_flags     = AI_PASSIVE;  /* For wildcard IP address */
    hints.ai_protocol  = 0;           /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr      = NULL;
    hints.ai_next      = NULL;
    
    int retval = getaddrinfo(hostname, NULL, &hints, &result);
    if ((retval != 0) || (NULL == result))
    {
        errinfo = gai_strerror(retval);
        if (result != NULL)
            freeaddrinfo(result);
        return false;
    }

    for (struct addrinfo* addr=result; addr!=NULL; addr=addr->ai_next)
    {
        if (AF_INET == addr->ai_family)
        {
            struct sockaddr_in* addr_in = (struct sockaddr_in*)(addr->ai_addr);
            (void)inet_ntop(addr->ai_family, &addr_in->sin_addr.s_addr, ip, sizeof(ip));
            ip_array.push_back(ip);
        }
        else if (AF_INET6 == addr->ai_family)
        {            
            struct sockaddr_in6* addr_in6 = (struct sockaddr_in6*)(addr->ai_addr);
            (void)inet_ntop(addr->ai_family, addr_in6->sin6_addr.s6_addr32, ip, sizeof(ip));
            ip_array.push_back(ip);
        }               
    }
    
    freeaddrinfo(result);
    return ip_array.size() > 0;
}

//#include <net/if.h>
/* Structure used in SIOCGIFCONF request.  Used to retrieve interface
configuration for machine (useful for programs which must know all
networks accessible).  */
// struct ifconf
// {
//     int ifc_len;                        /* Size of buffer.  */
//     union
//     {
//         __caddr_t ifcu_buf;
//         struct ifreq *ifcu_req;
//     } ifc_ifcu;
// };

//struct ifreq 
//{
//#define IFHWADDRLEN     6
//        union   
//        {       
//                char    ifrn_name[IFNAMSIZ];            /* if name, e.g. "en0" */
//        } ifr_ifrn;
//        
//        union { 
//                struct  sockaddr ifru_addr;
//                struct  sockaddr ifru_dstaddr;
//                struct  sockaddr ifru_broadaddr;
//                struct  sockaddr ifru_netmask;
//                struct  sockaddr ifru_hwaddr;
//                short   ifru_flags;
//                int     ifru_ivalue;
//                int     ifru_mtu;
//                struct  ifmap ifru_map;
//                char    ifru_slave[IFNAMSIZ];   /* Just fits the size */
//                char    ifru_newname[IFNAMSIZ];
//                void __user *   ifru_data;
//                struct  if_settings ifru_settings;
//        } ifr_ifru;
//};
//
//#define ifr_name        ifr_ifrn.ifrn_name      /* interface name       */
//#define ifr_hwaddr      ifr_ifru.ifru_hwaddr    /* MAC address          */
//#define ifr_addr        ifr_ifru.ifru_addr      /* address              */
//#define ifr_dstaddr     ifr_ifru.ifru_dstaddr   /* other end of p-p lnk */
//#define ifr_broadaddr   ifr_ifru.ifru_broadaddr /* broadcast address    */
//#define ifr_netmask     ifr_ifru.ifru_netmask   /* interface net mask   */
//#define ifr_flags       ifr_ifru.ifru_flags     /* flags                */
//#define ifr_metric      ifr_ifru.ifru_ivalue    /* metric               */
//#define ifr_mtu         ifr_ifru.ifru_mtu       /* mtu                  */
//#define ifr_map         ifr_ifru.ifru_map       /* device map           */
//#define ifr_slave       ifr_ifru.ifru_slave     /* slave device         */
//#define ifr_data        ifr_ifru.ifru_data      /* for use by interface */
//#define ifr_ifindex     ifr_ifru.ifru_ivalue    /* interface index      */
//#define ifr_bandwidth   ifr_ifru.ifru_ivalue    /* link bandwidth       */
//#define ifr_qlen        ifr_ifru.ifru_ivalue    /* Queue length         */
//#define ifr_newname     ifr_ifru.ifru_newname   /* New name             */
//#define ifr_settings    ifr_ifru.ifru_settings  /* Device/proto settings*/
void CUtil::get_ethx_ip(eth_ip_array_t& eth_ip_array)
{	        
    struct ifconf ifc;   
    struct ifreq ifr[10]; // 最多10个IP
        
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == fd)
        throw sys::CSyscallException(errno, __FILE__, __LINE__);
    
    sys::CloseHelper<int> ch(fd);
    ifc.ifc_len = sizeof(ifr);
    ifc.ifc_buf = (caddr_t)&ifr[0];
    
    if (-1 == ioctl(fd, SIOCGIFCONF, (char*)&ifc))
        throw sys::CSyscallException(errno, __FILE__, __LINE__);
        
    // 计算网卡个数
    int ethx_count = ifc.ifc_len / sizeof(struct ifreq);     
    for (int i=0; i<ethx_count; ++i)
    {
        // 获取指定网卡上的IP地址
        if (-1 == ioctl(fd, SIOCGIFADDR, (char *)&ifr[i])) continue;
        if (NULL == ifr[i].ifr_name) continue;                

        const char* retval;
        char ip[IP_ADDRESS_MAX] = {0}; // IPV6 > IPV4

        // 转换
        if (AF_INET == ifr[i].ifr_addr.sa_family)
            retval = inet_ntop(AF_INET, &((struct sockaddr_in*)(&ifr[i].ifr_addr))->sin_addr, ip, sizeof(ip)-1);
        else
            retval = inet_ntop(AF_INET6, &((struct sockaddr_in*)(&ifr[i].ifr_addr))->sin_addr, ip, sizeof(ip)-1);

        if (NULL == retval)
        {
            int errcode = errno;                
            eth_ip_array.clear();
            throw sys::CSyscallException(errcode, __FILE__, __LINE__);
        }
     
        eth_ip_array.push_back(std::pair<std::string, std::string>(ifr[i].ifr_name, ip));                
    }
}

void CUtil::get_ethx_ip(const char* ethx, string_ip_array_t& ip_array)
{
    eth_ip_array_t eth_ip_array;
    get_ethx_ip(eth_ip_array);
       
    for (size_t i=0; i<eth_ip_array.size(); ++i)
    {
        if ((NULL == ethx) || (eth_ip_array[i].first == ethx))
        {
            ip_array.push_back(eth_ip_array[i].second);
        }            
    }
}

std::string CUtil::ipv4_tostring(uint32_t ipv4)
{
    char ip_address[IP_ADDRESS_MAX];

    if (NULL == inet_ntop(AF_INET, (struct in_addr*)&ipv4, ip_address, sizeof(ip_address)-1))
        ip_address[0] = '\0';

    return ip_address;
}

std::string CUtil::ipv6_tostring(const uint32_t* ipv6)
{
    char ip_address[IP_ADDRESS_MAX];
  
    if (NULL == ipv6)
    {
        ip_address[0] = '\0';
    }
    else
    {
        if (NULL == inet_ntop(AF_INET6, ipv6, ip_address, sizeof(ip_address)-1))
            ip_address[0] = '\0';
    }

    return ip_address;
}

bool CUtil::string_toipv4(const char* source, uint32_t& ipv4)
{    
    if (NULL == source) return false;
    return inet_pton(AF_INET, source, (void*)&ipv4) > 0;
}

bool CUtil::string_toipv6(const char* source, uint32_t* ipv6)
{       
    if ((NULL == source) || (NULL == ipv6)) return false;
    return inet_pton(AF_INET6, source, (void*)ipv6) > 0;
}

bool CUtil::is_ethx(const char* str)
{
    if (NULL == str) return false;
    
    if (strncmp(str, "eth", 3) != 0) return false;
    if ((str[3] >= '0') && (str[3] <= '9'))
        return str[4] == '\0';

    return false;
}

bool CUtil::is_broadcast_address(const char* str)
{
    return (NULL == str)? false: (0 == strcmp(str, "255.255.255.255"));
}

bool CUtil::timed_poll(int fd, int events_requested, int milliseconds, int* events_returned)
{
    int remaining_milliseconds = milliseconds;
    struct pollfd fds[1];
    fds[0].fd = fd;
    fds[0].events = events_requested; // POLLIN | POLLOUT | POLLERR;

    for (;;)
    {        
        time_t begin_seconds = time(NULL);
        int retval = poll(fds, sizeof(fds)/sizeof(struct pollfd), remaining_milliseconds);
        if (retval > 0)
            break;

        // 超时返回false
        if (0 == retval)
            return false;
        
        // 出错抛出异常
        if (-1 == retval)
        {
            // 中断，则继续
            if (EINTR == errno)
            {
                // 保证时间总是递减的，虽然会引入不精确问题，但总是好些，极端情况下也不会死循环                
                time_t gone_milliseconds = (time(NULL)-begin_seconds) * 1000 + 10;
                remaining_milliseconds = (remaining_milliseconds > gone_milliseconds)? remaining_milliseconds - gone_milliseconds: 0;                
                continue;
            }

            throw sys::CSyscallException(errno, __FILE__, __LINE__);
        }
    }

    if (events_returned != NULL) *events_returned = fds[0].revents;
    return true;
}

NET_NAMESPACE_END
