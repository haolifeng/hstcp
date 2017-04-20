/*
 * tcp_accept_thread.h
 *
 *  Created on: 2017年4月20日
 *      Author: hlf
 */

#ifndef TCP_ACCEPT_THREAD_H_
#define TCP_ACCEPT_THREAD_H_
#include <base/tcp_thread.h>
class CTcpServer;
class CConnectPool;
class CTcpAcceptThread:public CThread
{
public:
	CTcpAcceptThread(CTcpServer* lpServer);
	~CTcpAcceptThread();
	int Init();
	virtual long Run();
	int AddAcceptor(CTcpServer* lpServer);
protected:
	int m_hEpoll;
	CTcpServer* m_lpServer;
};



#endif /* TCP_ACCEPT_THREAD_H_ */
