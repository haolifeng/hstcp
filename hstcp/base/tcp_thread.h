/*
 * tcp_thread.h
 *
 *  Created on: 2017年4月14日
 *      Author: hlf
 */

#ifndef TCP_THREAD_H_
#define TCP_THREAD_H_

#include "tcp_base_connect.h"
#include <sys/thread.h>
///发送数据线程和接收数据线程的基类
class CTcpThread:public CThread
{
public:
	CTcpThread(uint32_t ev);
	~CTcpThread();
	///初始化线程类
	int Init();
	///将连接放入到线程的epoll中
	int AddConnect(CTcpBaseConnect*);
	///将连接从epoll中删除
	int DelConnect(CTcpBaseConnect*);

protected:
	///epoll的fd
	int			m_hEpoll;
	///加入本线程的连接关注的epoll事件
	uint32_t	m_nEpollEvent;
};

///负责发送数据（积压下来的）的线程类
class CTcpSendThread:public CTcpThread
{
public:
	CTcpSendThread():CTcpThread(EPOLLOUT|EPOLLET){}  //边缘触发
	///线程执行函数
	virtual long Run();
};

///负责接收数据的线程类
class CTcpRecvThread:public CTcpThread
{
public:
	CTcpRecvThread():CTcpThread(EPOLLIN){}   //水平触发
	///线程执行函数
	virtual long Run();
};




#endif /* TCP_THREAD_H_ */
