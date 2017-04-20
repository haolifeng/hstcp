#ifndef TCP_ACTIVE_CONNECT_H
#define TCP_ACTIVE_CONNECT_H
#include <base/tcp_base_connect.h>
//#include "tcp_client.h"
class CTcpClient;
class CActiveConnect: public CTcpBaseConnect
{
public:
	CActiveConnect(uint32_t nInitSendBuffSize, CTcpClient* lpInitiator);

public:
	//CTcpBaseConnect回调的接口函数实现
	virtual int OnConnected();
	virtual int OnDisconnect();
	virtual int OnRecvRawData(const char* lpData, uint32_t nLen);
	virtual int OnRecvBiz(const char* lpBizData, uint32_t nBizLen);
	virtual int OnError(int32_t iErrNo, const char* lpErrMsg);
	virtual bool IsConnected();
	virtual bool DealDisconnect();

	///设置连接状态
	void SetConnectStatus(bool status){m_bIsConnected=status;}
protected:
	///主动连接对象指针
	CTcpClient* m_lpTcpClient;
	///连接状态，是否连接上
	bool		m_bIsConnected;
};
#endif
