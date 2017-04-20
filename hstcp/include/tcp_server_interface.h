/*
 * server_interface.h
 *
 *  Created on: 2017年4月14日
 *      Author: hlf
 */

#ifndef SERVER_INTERFACE_H_
#define SERVER_INTERFACE_H_
#include <stdint.h>
#include <stdio.h>
class ITcpServerCallBack
{
public:
	virtual ~ITcpServerCallBack(){}
	virtual int OnAccepted(
								uint32_t nSockIndex,
								void**lppArg,
								const char* lpIPAddr,
								uint16_t wPort)=0;

   virtual int OnDisconnect(uint32_t nSockIndex, void* lpArg)=0;

	virtual int OnRecvRawData(
									uint32_t nSockIndex,
									void* lpArg,
									const char* lpData,
									uint32_t nLen)=0;

	virtual int OnRecvBiz(
								uint32_t nSockIndex,
								void* lpArg,
								const char* lpBizData,
								uint32_t nBizLen)=0;

	virtual int OnError(
							int32_t iErrNo,
							uint32_t nSockIndex,
							void* lpArg,
							const char* lpErrMsg)=0;

	virtual int OnHeartbeat(uint32_t nSockIndex, void* lpArg, uint32_t nTime)=0;

	virtual int OnTimeout(uint32_t nSockIndex, void* lpArg, uint32_t nTime)=0;
};
class ITcpServer{
public:
	virtual ~ITcpServer(){}
    virtual int SendData(
							uint32_t nSockIndex,
							const void* lpData,
							int32_t iDataLen,
							int32_t iWaitTime=500,
							uint32_t nBlockFlag=0)=0;

    virtual int Disconnect(uint32_t nSockIndex)=0;

    virtual int SetHeartbeat(
								uint32_t nSockIndex,
								int32_t iTime)=0;

    virtual int SetTimeout(uint32_t nSockIndex, int32_t iTime)=0;

    virtual int SetQuickAck(uint32_t nSockIndex, bool bQuickAck)=0;

    virtual int SetTcpNoDelay(uint32_t nSockIndex, bool bNoDelay)=0;

    virtual int SetTcpKeepAlive(uint32_t nSockIndex, bool bKeepAlive)=0;

	virtual int GetSendDataTimes()=0;

};
#ifdef __cplusplus
extern "C"{
#endif
int CreateTcpServer(
					ITcpServer** lppServer,
					const char* lpIpAddr,
					uint16_t wPort,
					ITcpServerCallBack* lpCallback,
					bool bReBind = 1,
					int32_t iSendBuffSize=512,
					int32_t iSendThreadCount=1,
					int32_t iRecvThreadCount=1);

int DestroyTcpServer(ITcpServer* lpServer);

#ifdef __cplusplus
}
#endif


#endif /* SERVER_INTERFACE_H_ */
