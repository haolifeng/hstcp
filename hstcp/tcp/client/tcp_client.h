/*
 * client.h
 *
 *  Created on: 2017年4月14日
 *      Author: hlf
 */

#ifndef TCP_CLIENT_H_
#define TCP_CLIENT_H_

#include <include/tcp_client_interface.h>
#include <sys/thread.h>
#include <base/tcp_base_connect.h>
#include <base/tcp_thread.h>
#include <sys/common.h>
#include "tcp_active_connect.h"
#include "tcp_client_creator.h"
///负责tcp连接的管理

class CTcpClient:public ITcpClient
{
public:
	friend class CTcpClientCreator;
	CTcpClient(
				ITcpClientCallBack* lpCallBack,
				uint32_t nSendBuffSize,
				bool bAutoReConnect );
	///初始化函数，必须调用
	int Init(	int32_t iSendIndex,
				int32_t iRecvIndex,
				const char* lpServerAddr,
				uint16_t wServerPort,
				const char* lpLocalAddr=NULL );

public:
	//应用层接口函数
	virtual int Connect(
						const char*	lpServerAddr=NULL,
						uint16_t	wServerPort=0,
						bool bBlockConnect=false,
						const char* lpLocalAddr=NULL );
	virtual int SendData(
						const void* lpData,
						int32_t iDataLen,
						int32_t iWaitTime,
						uint32_t nBlockFlag );
	virtual int Disconnect();
	virtual int ReConnect(
						const char*	lpServerAddr=NULL,
						uint16_t	wServerPort=0,
						bool bBlockConnect=false,
						const char* lpLocalAddr=NULL );

	virtual int SetHeartbeat(int32_t iTime);
	virtual int SetTimeout(int32_t iTime);
	virtual int SetQuickAck(bool bQuickAck);
	virtual int SetTcpNoDelay(bool bDelay);
	virtual int SetTcpKeepAlive(bool bKeepAlive);
	virtual int SetAutoReConnect(bool bAutoReConnect);
	virtual bool GetAutoReConnect();
	virtual bool GetConnectStatus();
	virtual int GetConnectCount();

public:
	///获取应用层的回调接口
	ITcpClientCallBack* GetCallback(){return m_lpCallback;}
	///获取连接次数
	uint32_t GetConnectTimes(){return m_nConnectTimes;}
	///创建套接字，发起tcp主动连接(非阻塞)
	int ConnectNonBlock();
	///创建套接字，发起tcp主动连接(阻塞模式)
	int ConnectBlock();
	///加入epoll线程中
	int Add2Epoll();
	///从epoll线程中删除
	int DelFromEpoll();
	///检查是否需要重连，每隔0.5秒调用一次
	bool NeedReConnect();
	///设置连接次数，用于连接上后重置连接次数
	int SetConnectTimes();
private:
	///设置套接字阻塞模式
	int SetNonBlocking(int nonblock);

protected:
	///主动连接对象的连接
	CActiveConnect		m_Connect;
	///应用层回调接口
	ITcpClientCallBack* m_lpCallback;
	///发送线程索引
	int32_t				m_iSendThreadIndex;
	///接收线程索引
	int32_t				m_iRecvThreadIndex;
	///连接的次数
	uint32_t			m_nConnectTimes;
	///检查是否需要重连次数
	uint32_t			m_nCheckReConnectTimes;
	///是否自动重连
	bool				m_bAutoReConnect;
	///本地网卡地址
	struct sockaddr_in	m_localaddr;
};


#endif /* CLIENT_H_ */
