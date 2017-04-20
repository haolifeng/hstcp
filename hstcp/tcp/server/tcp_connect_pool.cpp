#include "tcp_connect_pool.h"
#include "tcp_server.h"
CConnectPool::CConnectPool(CTcpServer* lpServer, uint32_t nInitBuffSize)
	:m_nCurConnectCount(0),m_nTotalConnectCount(0),m_lpHead(NULL),m_lpTail(NULL)
	,m_lpServer(lpServer),m_nInitBuffSize(nInitBuffSize)
{
}

CConnectPool::~CConnectPool()
{
	while(m_lpHead != NULL)
	{
		CAcceptConnect* lpConnect = m_lpHead->m_lpNext;
		delete m_lpHead;
		m_lpHead = lpConnect;
	}
}

int CConnectPool::Init()
{
	for(int i=0; i<CONNECT_POOL_INIT_COUNT; ++i)
	{
		//为了保证连接编号从小的开始，要把初始化连接编号的高16位设置为0xffff
		CAcceptConnect* lpConnect = NEW CAcceptConnect(
			m_nInitBuffSize,i|0xFFFF0000, m_lpServer->GetCallback(),this);
		if( lpConnect && !lpConnect->Init(-1, NULL) )
		{
			ReleaseConnect( lpConnect );
		}
		else
		{
			if( lpConnect )
			{
				delete lpConnect;
			}
			return TCP_ERROR_ALLOC_MEMORY_FAILED;
		}
	}
	m_nTotalConnectCount = CONNECT_POOL_INIT_COUNT;
	return 0;
}

int CConnectPool::ReleaseConnect(CAcceptConnect* lpConnect)
{
	m_ConnectPoolMutex.lock();
	if( m_lpTail )
	{
		m_lpTail->m_lpNext = lpConnect;
		m_lpTail = lpConnect;
	}
	else
	{
		m_lpHead = m_lpTail = lpConnect;
	}
	++m_nCurConnectCount;
	m_ConnectPoolMutex.unlock();
	return 0;
}

CAcceptConnect* CConnectPool::GetConnect()
{
	CAcceptConnect* lpConnect=NULL;
	m_ConnectPoolMutex.lock();
	if( m_lpHead )
	{
		lpConnect = m_lpHead;
		m_lpHead = lpConnect->m_lpNext;
		if( !m_lpHead )
		{
			m_lpTail = NULL;
		}
		lpConnect->m_lpNext = NULL;
		--m_nCurConnectCount;
	}
	//找不到连接并且连接总个数不超过MAX_ONLINE_CONNECT_COUNT时，新建一个连接
	if( (lpConnect==NULL) && (m_nTotalConnectCount<MAX_ONLINE_CONNECT_COUNT) )
	{
		lpConnect = NEW CAcceptConnect(m_nInitBuffSize,
			m_nTotalConnectCount|0xFFFF0000, m_lpServer->GetCallback(),this);
		if( lpConnect )
		{
			if( !lpConnect->Init(-1, NULL) )
			{
				++m_nTotalConnectCount;
			}
			else
			{
				delete lpConnect;
				lpConnect = NULL;
			}
		}
	}
	m_ConnectPoolMutex.unlock();
	//如果获取成功，修改连接编号的高16位
	if( lpConnect )
	{
		uint16_t nHighIndex = lpConnect->m_nIndex>>16;
		++nHighIndex;
		lpConnect->m_nIndex = nHighIndex<<16 | (lpConnect->m_nIndex&INDEX_LOW_MASK);
		lpConnect->m_nHeartbeatInterval = 0;
		lpConnect->m_nTimeoutInterval = 0;
	}
	return lpConnect;
}
