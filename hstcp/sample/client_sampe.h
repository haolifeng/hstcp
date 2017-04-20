/*
 * client_sampe.h
 *
 *  Created on: Apr 20, 2017
 *      Author: hlf
 */

#ifndef CLIENT_SAMPE_H_
#define CLIENT_SAMPE_H_

class CSampleClient :public ITcpClient
{
	virtual int Connect(const char* lpServerAddr = NULL,
			uint16_t wServerPort = 0,
			bool bBlockConnect = false,
			const char* lpLocalAddr = NULL) = 0;
	 virtual int SendData(
								const void* lpData,
								int32_t iDataLen,
								int32_t iWaitTime = 500,
								uint32_t nBlockFlag = 0 ) = 0;
	 virtual int Disconnect()=0;
	 virtual int ReConnect(
	 							const char*	lpServerAddr = NULL,
	 							uint16_t	wServerPort = 0,
	 							bool bBlockConnect = false,
	 							const char* lpLocalAddr = NULL ) = 0;
	 virtual int SetHeartbeat(int32_t iTime)=0;
	 virtual int SetTimeout(int32_t iTime)=0;
	 virtual int SetQuickAck(bool bQuickAck)=0;
	 virtual int SetTcpNoDelay(bool bNoDelay)=0;

	 virtual int SetTcpKeepAlive(bool bKeepAlive)=0;

	virtual int SetAutoReConnect(bool bAutoReConnect)=0;

	virtual bool GetAutoReConnect()=0;
	virtual bool GetConnectStatus()=0;

	virtual int GetConnectCount()=0;

};

class CSampleClientBackCall :public ITcpClientCallBack
{
	virtual int OnConnected()=0;

	virtual int OnDisconnect()=0;

	virtual int OnRecvRawData(const char* lpData, uint32_t nLen)=0;

	virtual int OnRecvBiz(const char* lpBizData, uint32_t nBizLen)=0;

	virtual int OnError(int32_t iErrNo, const char* lpErrMsg)=0;

	virtual int OnHeartbeat(uint32_t nTime)=0;

	virtual int OnTimeout(uint32_t nTime)=0;

};



#endif /* CLIENT_SAMPE_H_ */
