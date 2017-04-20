/*
 * tcp_accept_thread.cpp
 *
 *  Created on: 2017年4月20日
 *      Author: hlf
 */
#include "tcp_accept_thread.h"
#include "tcp_server.h"
#include "tcp_connect_pool.h"
CTcpAcceptThread::CTcpAcceptThread(CTcpServer* lpServer)
	:m_hEpoll(-1),m_lpServer(lpServer)
{
}

CTcpAcceptThread::~CTcpAcceptThread()
{
	close(m_hEpoll);
}

int CTcpAcceptThread::Init()
{
	m_hEpoll = epoll_create(EPOLL_SIZE);
	if( m_hEpoll < 0 )
	{
		//DBG(("epoll_create error:%d %s\n", errno, strerror(errno)));
		return TCP_ERROR_CREATE_EPOLL_FAILED;
	}
	return 0;
}
long CTcpAcceptThread::Run()
{
	uint32_t nCount=0;
	char errbuff[ERROR_BUFF_SIZE];
	THREAD_SIGNAL_BLOCK
	while(!isTerminated())
	{
		epoll_event ev[10];
		int nfds = epoll_wait(m_hEpoll, ev, sizeof(ev)/sizeof(epoll_event),
			EPOLL_WAIT_TIME);
		for(int i=0; i<nfds; ++i)
		{
			CTcpServer *lpServer = (CTcpServer*)ev[i].data.ptr;
			if(ev[i].events & EPOLLIN)
			{
				lpServer->OnAccept();
			}
			if( ev[i].events & EPOLLERR)
			{
				snprintf(errbuff,ERROR_BUFF_SIZE, "epoll wait error:0x%x",ev[i].events);
				lpServer->GetCallback()->OnError(OE_EPOLL_WAIT_EPOLLERR,
					(uint32_t)-1, NULL, errbuff);
			}
		}

		++nCount;
		//每隔0.12秒左右检查一次连接的状态
		if( (nCount%3) == 0 )
		{
			m_lpServer->CheckConnection();
		}
	}
	return 0;
}
int CTcpAcceptThread::AddAcceptor(CTcpServer *lpServer)
{
	int ret = TCP_ERROR_EPOLL_ADD_FAILED;
	char errbuff[ERROR_BUFF_SIZE];
	m_lpServer = lpServer;
	if( m_hEpoll != -1 && lpServer)
	{
		epoll_event ev;
		ev.events = EPOLLIN;
		ev.data.ptr = lpServer;
		ret = epoll_ctl(m_hEpoll, EPOLL_CTL_ADD, lpServer->GetSocket(), &ev);
		if( ret )
		{
			ret = TCP_ERROR_EPOLL_ADD_FAILED;
			snprintf(errbuff, ERROR_BUFF_SIZE,"epoll ctl add socket[%d]failed,errno:%d strerror:%s",
					lpServer->GetSocket(), errno, strerror(errno));
			lpServer->GetCallback()->OnError(OE_SYSTEM_ERROR,(uint32_t)-1, NULL, errbuff);
		}
	}
	return ret;
}

