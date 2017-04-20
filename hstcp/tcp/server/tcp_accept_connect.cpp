#include "tcp_accept_connect.h"
#include "tcp_connect_pool.h"
#include "tcp_server.h"
CAcceptConnect::CAcceptConnect(
	uint32_t nInitSendBuffSize,
	uint32_t nIndex,
	ITcpServerCallBack*lpCallback,
	CConnectPool*lpPool)
	:CTcpBaseConnect(nInitSendBuffSize), m_nIndex(nIndex),m_lpArg(NULL),
	m_lpCallback(lpCallback),m_lpPool(lpPool), m_lpNext(NULL)
{
}

int CAcceptConnect::Release()
{
	return m_lpPool->ReleaseConnect(this);
}

int CAcceptConnect::OnConnected()
{
	return 0;
}

int CAcceptConnect::OnDisconnect()
{
	m_lpCallback->OnDisconnect(m_nIndex, m_lpArg);
	this->Release();
	return 0;
}

int CAcceptConnect::OnRecvRawData(const char* lpData, uint32_t nLen)
{
	return m_lpCallback->OnRecvRawData(m_nIndex, m_lpArg, lpData, nLen);
}

int CAcceptConnect::OnRecvBiz(const char* lpBizData, uint32_t nBizLen)
{
	return m_lpCallback->OnRecvBiz(m_nIndex, m_lpArg, lpBizData, nBizLen);
}

int CAcceptConnect::OnError(int32_t iErrNo, const char* lpErrMsg)
{
	return m_lpCallback->OnError(iErrNo, m_nIndex, m_lpArg, lpErrMsg);
}

bool CAcceptConnect::IsConnected()
{
	return m_fd != -1;
}

bool CAcceptConnect::DealDisconnect()
{
	m_lpPool->m_lpServer->DelConnect(m_nIndex);
	return true;
}
