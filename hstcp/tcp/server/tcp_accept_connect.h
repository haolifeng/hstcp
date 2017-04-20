#ifndef TCP_ACCEPT_CONNECT_H
#define TCP_ACCEPT_CONNECT_H
#include <base/tcp_base_connect.h>
#include <include/tcp_server_interface.h>
#include "tcp_accept_connect.h"
class CConnectPool;
class CTcpServer;
class CAcceptConnect:public CTcpBaseConnect
{
public:
	friend class CConnectPool;
	CAcceptConnect(uint32_t nInitSendBuffSize,
			uint32_t nIndex,
			ITcpServerCallBack* lpCallback,
			CConnectPool* lpPool);
	int Release();
	virtual int OnConnected();
	virtual int OnDisconnect();
	virtual int OnRecvRawData(const char* lpData, uint32_t nLen);
	virtual int OnRecvBiz(const char* lpBizData, uint32_t nBizLen);
	virtual int OnError(int32_t iErrNo, const char* lpErrMsg);
	virtual bool IsConnected();
	virtual bool DealDisconnect();

	///获取连接编号
	uint32_t GetIndex(){return m_nIndex;}
	///获取存放应用层指针的指针
	void**	GetArgPointer(){return &m_lpArg;}

protected:
		///本链接对象的编号，编号分高16位和低16位
	uint32_t		m_nIndex;
		///在OnAccepted时，应用层设置的指针
	void*			m_lpArg;
		///回调应用层的回调接口
	ITcpServerCallBack*	m_lpCallback;
		///连接池
	CConnectPool*		m_lpPool;
		///在连接池中，指向下一个空闲链接
	CAcceptConnect*	m_lpNext;

};
#endif
