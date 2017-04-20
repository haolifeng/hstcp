/*
 * connect.h
 *
 *  Created on: 2017年4月14日
 *      Author: hlf
 */

#ifndef TCP_BASE_CONNECT_H_
#define TCP_BASE_CONNECT_H_

#include "tcp_lib_attr.h"
#include <sys/notify.h>
#include <sys/thread_mutex.h>
class CTcpBaseConnect{
public:
	CTcpBaseConnect(uint32_t nInitSendBuffSize);
	virtual ~CTcpBaseConnect();
	int Init(int fd,struct sockaddr_in* addr);
public:
	int SendData(const char* lpData,uint32_t nDataLen,int32_t iWaitTime,uint32_t nBlockFlag);
	int SendBlockData();
	int OnRecvData();
	int Disconnect();
public:
	int GetSocket(){return m_fd;}
	struct sockaddr_in* GetAddr(){return &m_addr;}
	int GetPeerIpAddr(char* buf,int sz);

	uint32_t GetHeartbeatInterval(){return m_nHeartbeatInterval;}
	uint32_t GetTimeoutInterval(){return m_nTimeoutInterval;}
	timeval* GetSendTimestamp(){return &m_nSendTimestamp;}
	timeval* GetRecvTimestamp(){return &m_nRecvTimestamp;}
	///获取时间差，返回为毫秒数
	int32_t GetTimeDiff(timeval* ts, timeval* te)
	{
		return ((te->tv_sec-ts->tv_sec)*1000000 + te->tv_usec - ts->tv_usec)/1000;
	}

	///设置心跳间隔和心跳类型，单位为秒
	int SetHeartbeat(int32_t iTime);
	///设置超时检测机制，单位为秒
	int SetTimeout(int32_t iTime);
	///设置QuickAck标志
	void SetQuickAck(int ack){m_iQuickAck=ack;}
public:
	//以下接口是回调应用层的接口，对主动连接和被动侦听需要不同的实现
	///主动连接才会触发该回调接口
	virtual int OnConnected()=0;
	///回调应用层通知连接断开事件，被动侦听释放资源
	virtual int OnDisconnect()=0;
	///仅仅回调应用层
	virtual int OnRecvRawData(const char* lpData, uint32_t nLen)=0;
	///仅仅回调应用层
	virtual int OnRecvBiz(const char* lpBizData, uint32_t nBizLen)=0;
	///仅仅回调应用层
	virtual int OnError(int32_t iErrNo, const char* lpErrMsg)=0;

	///获取连接状态
	virtual bool IsConnected()=0;
	///返回是否需要回调OnDisconnect
	virtual bool DealDisconnect()=0;

private:
	///立即发送业务层的数据
	int SendDataImmediate(const char* lpData, uint32_t nDataLen);
	///将阻塞的发送数据放入发送缓存中
	int AddSendDataToBuf(const char* lpData, uint32_t nDataLen);
	///有积压数据时，调用该函数发送数据，可能等待iWaitTime秒
	int SendDataWait(const char* lpData,
						uint32_t nDataLen,
						int32_t iWaitTime,
						uint32_t nBlockFlag,
						bool& bReSend );
	///处理接收到的数据
	int DealRecvData(int iRecvLen, char errbuff[]);
protected:
	CNotify   m_SendDataNotify;
	CThreadMutex m_SendDataMutex;
	uint32_t m_nSendDataOffset;
	uint32_t m_nSendDataLen;
	uint32_t m_nSendBuffSize;
	char*  m_lpSendBuff;
	CThreadMutex  m_RecvDataMutex;
	int32_t m_iPacketLen;
	uint32_t m_nRecvDataLen;
	char  m_lpRecvBuff[RECV_BUFF_SIZE];

	uint32_t m_nInitSendBuffSize;
	struct sockaddr_in m_addr;
	int m_fd;
	timeval m_nSendTimestamp;
	timeval m_nRecvTimestamp;
	uint32_t m_nHeartbeatInterval;
	uint32_t m_nTimeoutInterval;
	int m_iQuickAck;

};



#endif /* CONNECT_H_ */
