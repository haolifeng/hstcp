/*
 * tcp_lib_attr.h
 *
 *  Created on: 2017年4月14日
 *      Author: hlf
 */

#ifndef TCP_LIB_ATTR_H_
#define TCP_LIB_ATTR_H_
#include <sys/common.h>
const int EPOLL_SIZE = 2000;
const int EPOLL_WAIT_TIME = 40;
const int MIN_THREAD_COUNT =1;
const int MAX_THREAD_COUNT = 16;
const int MAX_WAIT_TIME = 3600000; // 1小时
const uint32_t  BLOCK_FLAG_OVERFLOW  = 1;
const uint32_t MAX_RECV_TIMES  = 30; //一次EPOLLIN事件,套接字最多recv的次数

const int ERROR_BUFF_SIZE  = 512;
const int RECV_BUFF_SIZE = (65*1024);  //65k



//参数错误
const int TCP_ERROR_WRONG_PARAM						= -1;
//未初始化
const int TCP_ERROR_NOT_INIT						= -2;
//已经初始化过，不能再次初始化
const int TCP_ERROR_INIT_AGAIN						= -3;
//内存分配失败
const int TCP_ERROR_ALLOC_MEMORY_FAILED				= -4;
//创建Epoll错误
const int TCP_ERROR_CREATE_EPOLL_FAILED				= -5;
//创建socket错误
const int TCP_ERROR_CREATE_SOCKET_FAILED			= -6;
//绑定错误
const int TCP_ERROR_BIND_FAILED						= -7;
//连接错误
const int TCP_ERROR_CONNECT_FAILED					= -8;
//将套接字放入EPOLL错误
const int TCP_ERROR_EPOLL_ADD_FAILED				= -9;
//连接未建立或者连接已经断开
const int TCP_ERROR_NOT_ESTABLISHED					= -10;
//发送数据错误
const int TCP_ERROR_SEND_ERROR						= -11;
//发送数据超时仍未发出
const int TCP_ERROR_SEND_TIMEOUT					= -12;
//设置套接字选项错误
const int TCP_ERROR_SETSOCKOPT_FAILED				= -13;
//listen错误
const int TCP_ERROR_LISTEN_FAILED					= -14;
//创建过多连接
const int TCP_ERROR_CREATE_TOO_MUCH					= -15;
//连接超时
const int TCP_ERROR_CONNECT_TIMEOUT					= -16;


//OnError回调接口返回的iErrNo参数定义
#define		OE_BASE						921000
const int  OE_SYSTEM_ERROR				=OE_BASE+1;
const int  OE_EPOLL_WAIT_EPOLLERR		=OE_BASE+2;
const int  OE_RECV_DATA_OVERFLOW		=OE_BASE+3;
const int  OE_ALLOC_MEMORY_FAILED		=OE_BASE+4;
const int  OE_CONNECT_FAILED			=OE_BASE+5;
const int  OE_CONNECTION_TIMEOUT		=OE_BASE+6;

#define THREAD_SIGNAL_BLOCK \
{\
	sigset_t set;\
	sigemptyset(&set);\
	sigaddset(&set, SIGINT);\
	sigaddset(&set, SIGTERM);\
	pthread_sigmask(SIG_BLOCK, &set, NULL);\
}

#define INDEX_LOW_MASK  0xFFFF
const int CONNECT_POOL_INIT_COUNT = 100;
const uint32_t MAX_ONLINE_CONNECT_COUNT = 10000;


#endif /* NET_ATTRI_CONF_H_ */
