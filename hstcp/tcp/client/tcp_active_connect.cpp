#include "tcp_active_connect.h"
#include "tcp_client.h"
CActiveConnect::CActiveConnect(uint32_t nInitSendBuffSize, CTcpClient* lpTcpClient)
	:CTcpBaseConnect(nInitSendBuffSize), m_lpTcpClient(lpTcpClient), m_bIsConnected(false)
{
}

int CActiveConnect::OnConnected()
{
	bool bCallback=false;
	int ret = 0;
	m_RecvDataMutex.lock();
	m_SendDataMutex.lock();
	//需要判断套接字和连接状态
	if( m_fd != -1 && !m_bIsConnected )
	{
		bCallback=true;
		m_bIsConnected = true;
		m_nSendDataOffset = 0;
		m_nSendDataLen = 0;
		m_iPacketLen = 0;
		m_nRecvDataLen	=0;
		m_SendDataNotify.Reset();
		gettimeofday(&m_nSendTimestamp, NULL);
		gettimeofday(&m_nRecvTimestamp, NULL);
	}
	m_lpTcpClient->SetConnectTimes();
	m_SendDataMutex.unlock();
	m_RecvDataMutex.unlock();
	//只有在原来连接是断开时，才回调应用层，保证只回调一次
	if( bCallback )
	{
		ret = m_lpTcpClient->GetCallback()->OnConnected();
	}
	return ret;
}

int CActiveConnect::OnDisconnect()
{
	return m_lpTcpClient->GetCallback()->OnDisconnect();
}

int CActiveConnect::OnRecvRawData(const char* lpData, uint32_t nLen)
{
	return m_lpTcpClient->GetCallback()->OnRecvRawData(lpData, nLen);
}

int CActiveConnect::OnRecvBiz(const char* lpBizData, uint32_t nBizLen)
{
	return m_lpTcpClient->GetCallback()->OnRecvBiz(lpBizData, nBizLen);
}

int CActiveConnect::OnError(int32_t iErrNo, const char* lpErrMsg)
{
	return m_lpTcpClient->GetCallback()->OnError(iErrNo, lpErrMsg);
}

bool CActiveConnect::IsConnected()
{
	return m_bIsConnected;
}

bool CActiveConnect::DealDisconnect()
{
	if( m_bIsConnected )
	{
		m_lpTcpClient->DelFromEpoll();
		m_bIsConnected = false;
		return true;
	}
	return false;
}
