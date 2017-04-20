/*
 * connect.cpp
 *
 *  Created on: 2017年4月14日
 *      Author: hlf
 */
#include "tcp_base_connect.h"

CTcpBaseConnect::CTcpBaseConnect(uint32_t nInitSendBuffSize)
:m_nSendDataOffset(0),m_nSendDataLen(0),m_lpSendBuff(NULL),m_iPacketLen(0),
 m_nRecvDataLen(0),m_nInitSendBuffSize(nInitSendBuffSize),m_fd(0),m_nHeartbeatInterval(0),
 m_nTimeoutInterval(0),m_iQuickAck(0)
{
    memset(m_lpRecvBuff, '\0', sizeof(m_lpRecvBuff));
}

CTcpBaseConnect::~CTcpBaseConnect()
{
	if( m_lpSendBuff )
	{
		free(m_lpSendBuff);
	}
	close(m_fd);
}
int CTcpBaseConnect::Init(int fd ,sockaddr_in *addr)
{
	//如果第一次初始化，则分配发送缓冲内存
	if( m_lpSendBuff == NULL )
	{
		m_lpSendBuff = (char*)malloc(m_nInitSendBuffSize);
		if( m_lpSendBuff == NULL )
		{
			return TCP_ERROR_ALLOC_MEMORY_FAILED;
		}
		m_nSendBuffSize = m_nInitSendBuffSize;
	}
	m_fd = fd;
	if( addr != NULL )
	{
		m_addr = *addr;
	}
	else
	{
		memset(&m_addr, 0, sizeof(m_addr));
	}
	m_SendDataNotify.Reset();
	m_nSendDataOffset = 0;
	m_nSendDataLen = 0;
	m_iPacketLen = 0;
	m_nRecvDataLen = 0;
	m_iQuickAck = 0;

	gettimeofday(&m_nSendTimestamp, NULL);
	gettimeofday(&m_nRecvTimestamp, NULL);

	return 0;
}
int CTcpBaseConnect::SendDataImmediate(const char* lpData,	uint32_t nDataLen )
{
	int ret=0;
	while(true)
	{
		int sendlen = send(m_fd, lpData, nDataLen, MSG_NOSIGNAL);
		if( sendlen == (int)nDataLen )  //如果发送完毕
		{
			gettimeofday(&m_nSendTimestamp, NULL);  //更新发送时间戳
		}
		else if( sendlen >= 0 )  //如果没有发送完毕，
		{
			gettimeofday(&m_nSendTimestamp, NULL);  //更新发送时间戳
			ret = AddSendDataToBuf(lpData+sendlen, nDataLen-sendlen); //将没有发送完毕的数据放入发送缓冲区中
		}
		else if( errno == EAGAIN )  //如果返回的错误为eagain,说明内核网络协议栈是满的
		{
			ret = AddSendDataToBuf(lpData, nDataLen); //将没有发送完毕的数据放入发送缓冲区中
		}
		else if( errno == EINTR )   //如果返回错误为eintr，也就是在数据发送之前接受到一个信号
		{
			continue;   //则重新发送一次
		}
		else   //如果是其他情况则，返回错误。
		{
			ret = TCP_ERROR_SEND_ERROR;
		}
		break;
	}
	return ret;
}
int CTcpBaseConnect::AddSendDataToBuf(const char* lpData, uint32_t nDataLen)
{
	//发送数据超出缓存中的剩余空间总和，扩大缓存nDataLen字节，避免memmove内存
	if( nDataLen > m_nSendBuffSize-m_nSendDataLen )
	{
		char* lpBuff = (char*)realloc(m_lpSendBuff, m_nSendBuffSize+nDataLen);
		if( lpBuff == NULL )
		{
			//DBG(("%s:%d %s alloc memory failed\n",__FILE__,__LINE__,__FUNCTION__));
			return TCP_ERROR_ALLOC_MEMORY_FAILED;
		}
		m_lpSendBuff = lpBuff;
		m_nSendBuffSize += nDataLen;
		memcpy(m_lpSendBuff+m_nSendDataOffset+m_nSendDataLen, lpData, nDataLen);
		m_nSendDataLen += nDataLen;
	}
	else
	{
		if( m_nSendDataLen == 0 )
		{
			m_nSendDataOffset = 0;
		}
		//如果发送缓存后面的空闲内存不足容纳nDataLen时，把发送积压数据移到m_lpSendBuff头部
		if( m_nSendDataOffset &&
			(nDataLen>m_nSendBuffSize-m_nSendDataOffset-m_nSendDataLen) )
		{
			memmove(m_lpSendBuff, m_lpSendBuff+m_nSendDataOffset, m_nSendDataLen);
			m_nSendDataOffset = 0;
		}
		memcpy(m_lpSendBuff+m_nSendDataOffset+m_nSendDataLen, lpData, nDataLen);
		m_nSendDataLen += nDataLen;
	}
	return 0;
}
int CTcpBaseConnect::SendDataWait(
	const char* lpData,
	uint32_t nDataLen,
	int32_t iWaitTime,
	uint32_t nBlockFlag,
	bool& bReSend )
{
	int ret = TCP_ERROR_NOT_ESTABLISHED;
	//积压的数据长度加上nDataLen超过m_nInitSendBuffSize
	if( m_nSendDataLen+nDataLen > m_nInitSendBuffSize )
	{
		m_SendDataMutex.unlock();
		int ev = m_SendDataNotify.Wait(iWaitTime);
		iWaitTime = 0;
		m_SendDataMutex.lock();
		//重新加锁，需要判断连接状态
		if( IsConnected() )
		{
			ret = 0;
			if( ev == NOTIFY_OK )//如果等到通知
			{
				bReSend = true;//有数据发送，重新发送一次
			}
			else
			{
				//事件可能唤醒的晚了
				if( !m_nSendDataLen )      //如果缓存中的数据发送完毕了
				{
					bReSend = true;       //重新发送
				}
				//此时发送线程没有发送任何数据，需要根据nBlockFlag处理
				else if( nBlockFlag & BLOCK_FLAG_OVERFLOW )//BLOCK_FLAG_OVERFLOW = 1  ？？？？
				{
					ret = TCP_ERROR_SEND_TIMEOUT;
				}
				else
				{
					ret = AddSendDataToBuf(lpData, nDataLen); //将数据添加的缓存中
				}
			}
		}
	}
	else
	{
		ret = AddSendDataToBuf(lpData, nDataLen);   //直接将数据添加到缓存中
	}
	return ret;
}
int CTcpBaseConnect::SendData(
						const char* lpData,
						uint32_t nDataLen,
						int32_t iWaitTime,
						uint32_t nBlockFlag)
{
	int ret=TCP_ERROR_NOT_ESTABLISHED;
	m_SendDataMutex.lock();
	//此时，可能该连接已经断开，需要判断连接是否正常
	if( IsConnected() )
	{
		bool bContinue;
		do{
			bContinue = false;
			if( !m_nSendDataLen)
			{
				//如果发送没有积压，则直接发送
				ret = SendDataImmediate(lpData, nDataLen);
			}
			else
			{
				//有积压数据
				ret = SendDataWait(lpData, nDataLen, iWaitTime, nBlockFlag, bContinue);
			}
		}while(bContinue);
	}
	m_SendDataMutex.unlock();
	return ret;
}
int CTcpBaseConnect::SendBlockData()
{
	int ret = 0;
	char errbuff[ERROR_BUFF_SIZE];
	m_SendDataMutex.lock();
	//此时，需要判断一下连接状态，以免连接此时已经断开
	if( IsConnected() && m_nSendDataLen )
	{
		while(true)
		{
			int sendlen = send(m_fd,m_lpSendBuff+m_nSendDataOffset, m_nSendDataLen,MSG_NOSIGNAL);
			if( sendlen >= 0 )
			{
				gettimeofday(&m_nSendTimestamp, NULL);
				m_SendDataNotify.Signal();
				m_nSendDataLen -= sendlen; //更新缓存中的数据长度
				if( !m_nSendDataLen ) //如果发送完毕
				{
					m_nSendDataOffset = 0;
				}
				else
				{
					m_nSendDataOffset += sendlen; //更新偏移量
				}
			}
			else if( errno == EINTR )
			{
				continue;
			}
			else if( errno != EAGAIN )
			{
				snprintf(errbuff,ERROR_BUFF_SIZE,"send data error,errno:%d strerror:%s",
					errno, strerror(errno));
				OnError(OE_SYSTEM_ERROR, errbuff);
				ret = -1;
			}
			break;
		}
	}
	m_SendDataMutex.unlock();
	return ret;
}
int CTcpBaseConnect::DealRecvData(int iRecvLen, char errbuff[])
{
	//保证主动连接套接字在recv之前先调用OnConnected函数
	if( !IsConnected() )
	{
		m_RecvDataMutex.unlock();
		int oc = OnConnected();
		m_RecvDataMutex.lock();
		if( oc )
		{
			return -1;
		}
	}
	gettimeofday(&m_nRecvTimestamp, NULL);
	m_nRecvDataLen += iRecvLen;
	int32_t iDataSize = m_nRecvDataLen;
	int32_t iDataOffset = 0;
	while(iDataSize)
	{
		if( !m_iPacketLen )
		{
			m_iPacketLen = OnRecvRawData(m_lpRecvBuff+iDataOffset, iDataSize);
		}
		if( (m_iPacketLen < 0) || (m_iPacketLen>RECV_BUFF_SIZE) ||
			((iDataSize==RECV_BUFF_SIZE) && (m_iPacketLen==0)) )
		{
			//数据包长度超过接收缓存长度时，向应用层返回出错
			//if( m_iPacketLen >=0 )
			{
				snprintf(errbuff,ERROR_BUFF_SIZE,"recv buffer overflow,buffer size:%d,receive data size:%d,packet len:%d",
					RECV_BUFF_SIZE, iDataSize, m_iPacketLen );
				OnError(OE_RECV_DATA_OVERFLOW, errbuff);
			}
			return -1;
		}
		else if( m_iPacketLen && (m_iPacketLen <= iDataSize) )
		{
			m_RecvDataMutex.unlock();
			OnRecvBiz(m_lpRecvBuff+iDataOffset, m_iPacketLen);
			m_RecvDataMutex.lock();
			//连接断开的话，直接退出
			if(!IsConnected())
			{
				return -1;
			}
			iDataOffset += m_iPacketLen;
			iDataSize -= m_iPacketLen;
			m_iPacketLen = 0;
			continue;
		}
		break;
	}
	if( iDataOffset )
	{
		if( m_nRecvDataLen > (uint32_t)iDataOffset )
		{
			memmove(m_lpRecvBuff, m_lpRecvBuff+iDataOffset, m_nRecvDataLen-iDataOffset);
		}
		m_nRecvDataLen -= iDataOffset;
	}
	return 0;
}
int CTcpBaseConnect::OnRecvData()
{
	int ret = 0;
	char errbuff[ERROR_BUFF_SIZE];
	uint32_t nRecvTimes=0;
	m_RecvDataMutex.lock();
	//防止某个套接字一直接收数据，其他套接字饿死
	while( ++nRecvTimes<=MAX_RECV_TIMES )
	{
		int recvlen = recv(m_fd,m_lpRecvBuff+m_nRecvDataLen,
			RECV_BUFF_SIZE-m_nRecvDataLen,0);
		if( m_iQuickAck )
		{
			setsockopt(m_fd, IPPROTO_TCP, TCP_QUICKACK, &m_iQuickAck, sizeof(int));
		}
		if (recvlen > 0)
		{
			ret = DealRecvData(recvlen, errbuff);
			if( ret == 0 )
			{
				continue;
			}
		}
		else if( recvlen == 0 )
		{
			ret = -1;
		}
		else if( errno == EINTR )
		{
			continue;
		}
		else if( errno != EAGAIN )
		{
			ret = -1;
			snprintf(errbuff,ERROR_BUFF_SIZE,"recv data error,errno:%d strerror:%s",
				errno, strerror(errno));
			OnError(OE_SYSTEM_ERROR, errbuff);
		}
		break;
	}
	m_RecvDataMutex.unlock();
	return ret;
}
int CTcpBaseConnect::Disconnect()
{
	bool bCallback = false;
	m_RecvDataMutex.lock();
	m_SendDataMutex.lock();
	if( m_fd != -1 )
	{
		bCallback = DealDisconnect();
		//关闭套接字
		close(m_fd);
		m_fd = -1;
		//清空发送
		m_nSendDataLen = 0;
		m_nSendDataOffset = 0;
		m_iPacketLen = 0;
		m_nRecvDataLen = 0;
		//唤醒发送等待的操作
		m_SendDataNotify.Signal();
	}
	m_SendDataMutex.unlock();
	m_RecvDataMutex.unlock();

	//此时，可能用户发送线程、发送线程或者接收线程正在处理
	if( bCallback )
	{
		OnDisconnect();
	}
	return 0;
}

int CTcpBaseConnect::GetPeerIpAddr(char* buf, int sz)
{
	uint32_t ipAddr = ntohl(m_addr.sin_addr.s_addr);
	snprintf(buf, sz, "%d.%d.%d.%d",
			(unsigned int)(ipAddr >>24)&0xff,
			(unsigned int)(ipAddr >>16)&0xff,
			(unsigned int)(ipAddr >> 8)&0xff,
			(unsigned int)(ipAddr &0xff));
	buf[sz-1] = 0;
	return 0;
}

int CTcpBaseConnect::SetHeartbeat(int32_t iTime)
{
	if( iTime>0 )
	{
		m_nHeartbeatInterval = iTime*1000;
	}
	else
	{
		m_nHeartbeatInterval = 0;
	}
	return 0;
}

int CTcpBaseConnect::SetTimeout(int32_t iTime)
{
	if( iTime>0 )
	{
		m_nTimeoutInterval = iTime*1000;
	}
	else
	{
		m_nTimeoutInterval = 0;
	}
	return 0;
}
