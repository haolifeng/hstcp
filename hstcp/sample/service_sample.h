/*
 * service_sample.h
 *
 *  Created on: Apr 20, 2017
 *      Author: hlf
 */

#ifndef SERVICE_SAMPLE_H_
#define SERVICE_SAMPLE_H_
#include <tcp_server_interface.h>

class CSampleServerCallBack: public ITcpServerCallBack
{
public:
	int OnAccepted(uint32_t nSockIndex,void**lppArg,const char* lpIPAddr,uint16_t wPort);

    int OnDisconnect(uint32_t nSockIndex, void* lpArg);

	int OnRecvRawData(uint32_t nSockIndex,void* lpArg,const char* lpData,uint32_t nLen);

	int OnRecvBiz(uint32_t nSockIndex,void* lpArg,const char* lpBizData,uint32_t nBizLen);

	int OnError(int32_t iErrNo,uint32_t nSockIndex,void* lpArg,const char* lpErrMsg);

	int OnHeartbeat(uint32_t nSockIndex, void* lpArg, uint32_t nTime);

	int OnTimeout(uint32_t nSockIndex, void* lpArg, uint32_t nTime);
};
class CSampleServer: public ITcpServer
{
public:
	int SendData(uint32_t nSockIndex,const void* lpData,int32_t iDataLen,int32_t iWaitTime=500,uint32_t nBlockFlag=0);

	int Disconnect(uint32_t nSockIndex);

	int SetHeartbeat(uint32_t nSockIndex,int32_t iTime,uint32_t nTriggerType=0);

	int SetTimeout(uint32_t nSockIndex, int32_t iTime);

	int SetQuickAck(uint32_t nSockIndex, bool bQuickAck);

	int SetTcpNoDelay(uint32_t nSockIndex, bool bNoDelay);

	int SetTcpKeepAlive(uint32_t nSockIndex, bool bKeepAlive);

	int GetSendDataTimes();
};




#endif /* SERVICE_SAMPLE_H_ */
