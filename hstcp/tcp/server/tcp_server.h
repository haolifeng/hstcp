#ifndef SERVER_H
#define SERVER_H
#include <include/tcp_server_interface.h>
#include <base/tcp_base_connect.h>
#include <sys/atomic_count.h>
#include <sys/thread.h>
#include <base/tcp_thread.h>
#include "tcp_accept_connect.h"
#include "tcp_accept_thread.h"
#include "tcp_connect_pool.h"
///主动侦听对象的类
class CTcpServer :public ITcpServer
{
public:
	CTcpServer();
	~CTcpServer();
	int Init(
			const char* lpIpAddr,
			uint16_t wPort,
			ITcpServerCallBack* lpCallback,
			bool bReBind,
			int32_t iSendBuffSize,
			int32_t iSendThreadCount,
			int32_t iRecvThreadCount );

public:
	//应用层接口
	virtual int SendData(
						uint32_t nSockIndex,
						const void* lpData,
						int32_t iDataLen,
						int32_t iWaitTime,
						uint32_t nBlockFlag );
	virtual int Disconnect(uint32_t nSockIndex);

	virtual int SetHeartbeat(uint32_t nSockIndex, int32_t iTime);
	virtual int SetTimeout(uint32_t nSockIndex, int32_t iTime);
	virtual int SetQuickAck(uint32_t nSockIndex, bool bQuickAck);
	virtual int SetTcpNoDelay(uint32_t nSockIndex, bool bDelay);
	virtual int SetTcpKeepAlive(uint32_t nSockIndex, bool bKeepAlive);
	virtual int GetSendDataTimes(){return m_iSendTimes;}

public:
	///当接收连接线程有连接接收时，调用该函数
	int OnAccept();
	///获取监听套接字
	int GetSocket(){return m_fd;}
	///获取监听的端口号
	int GetPort(){return ntohs(m_addr.sin_port);}
	///获取应用层注册的回调函数
	ITcpServerCallBack* GetCallback(){return m_lpCallback;}
	///获取接收连接线程
	CTcpAcceptThread* GetAcceptThread(){return m_lpAcceptThread;}

	///从活动连接中删除连接
	int DelConnect(uint32_t index);
	///检查连接状态
	int CheckConnection();
private:
	///创建套接字，绑定并开启监听
	int Listen(bool bReBind);
	///设置套接字为非阻塞模式
	int SetNonBlocking(int fd, int nonblock);
	///设置监听套接字是否可重用
	int SetReuseAddr(int reuse);
	///创建侦听、发送和接收线程并启动
	int CreateThread();

protected:
	///接收连接线程对象指针
	CTcpAcceptThread* m_lpAcceptThread;
	///发送数据线程个数
	int m_iSendThreadCount;
	///发送数据线程对象数组指针
	CTcpSendThread* m_lpSendThread;
	///接收数据线程个数
	int m_iRecvThreadCount;
	///接收数据线程对象数组指针
	CTcpRecvThread* m_lpRecvThread;

	///连接数组，存储在线连接
	CAcceptConnect*	m_lpConnectArray[MAX_ONLINE_CONNECT_COUNT];
	///连接池，存储空闲连接
	CConnectPool*		m_lpConnectPool;

	///监听套接字
	int					m_fd;
	///监听套接字绑定的地址和端口
	struct sockaddr_in	m_addr;
	///应用层注册的回调接口
	ITcpServerCallBack*	m_lpCallback;
	///创建acceptor时，传入的发送缓冲区字节数
	uint32_t			m_nInitSendBuffSize;
	///是否支持重复绑定
	bool				m_bReBind;
	///成功SendData次数
	atomic_count		m_iSendTimes;
};


#endif
