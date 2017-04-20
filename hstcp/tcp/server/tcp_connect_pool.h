#ifndef TCP_CONNECT_POOL_H
#define TCP_CONNECT_POOL_H
#include "tcp_accept_connect.h"
#include <sys/thread_mutex.h>

///连接池，负责管理被动连接
class CTcpServer;
class CConnectPool
{
public:
	CConnectPool(CTcpServer* lpAcceptor, uint32_t nInitBuffSize);
	~CConnectPool();
	///初始化该连接池
	int Init();

	///获取一个空闲连接
	CAcceptConnect* GetConnect();
	///释放一个不再使用的连接
	int ReleaseConnect(CAcceptConnect* lpConnect);

protected:
	CThreadMutex		m_ConnectPoolMutex;
	///当前池中空闲连接个数
	uint16_t			m_nCurConnectCount;
	///连接总个数，包括正在使用中的连接和空闲连接
	uint16_t			m_nTotalConnectCount;
	///连接队列头指针
	CAcceptConnect*	m_lpHead;
	///连接队列尾指针
	CAcceptConnect*	m_lpTail;
public:
	///本池属于的acceptor对象
	CTcpServer*			m_lpServer;
	///连接初始化发送缓冲区的字节数
	uint32_t			m_nInitBuffSize;
};
#endif
