#ifndef __GLOBAL_H__
#define __GLOBAL_H__

// 系统
#include <stdio.h>                  // I/O
#include <iostream>                 // I/O
#include <stdlib.h>                  // malloc free
#include <stdarg.h>                 // 变参
#include <memory.h>                 // 内存管理
#include <signal.h>                 // signal warning error.
#include <unistd.h>                 // usleep
#include <errno.h>                  // errno
#include <netinet/in.h>             // sockaddr_in
#include <arpa/inet.h>              // inet_addr
#include <time.h>
#include <sys/time.h>               // 时间
#include <sys/resource.h>           // *setrlimit */
#include <sys/epoll.h>              // epoll function
#include <fcntl.h>
#include <queue>                    // STL queue队列

#include "Timeval.h"

using namespace std;

#define SERVER_IP                   "127.0.0.1"
#define SERVER_PORT                 9000

#define GLODEN_KEY                  (273565573)
#define SIZE_PACKET_MAX             (1024 * 10)

#define SERVER_CMD_TEST             99

#define __DEBUG

#ifdef __DEBUG
#define LOG(format, args...)              printf((format), ##args)
#else
#define LOG(format, args...)
#endif

struct stuThreadParam
{
    int* pSocket;
    void* pBuffer;

    bool* pOnOff;
};

#endif // GLOBAL_H
