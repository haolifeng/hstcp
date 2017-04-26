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
	int Init();
	int OnAccepted(uint32_t nSockIndex,void**lppArg,const char* lpIPAddr,uint16_t wPort);

    int OnDisconnect(uint32_t nSockIndex, void* lpArg);

	int OnRecvRawData(uint32_t nSockIndex,void* lpArg,const char* lpData,uint32_t nLen);

	int OnRecvBiz(uint32_t nSockIndex,void* lpArg,const char* lpBizData,uint32_t nBizLen);

	int OnError(int32_t iErrNo,uint32_t nSockIndex,void* lpArg,const char* lpErrMsg);

	int OnHeartbeat(uint32_t nSockIndex, void* lpArg, uint32_t nTime);

	int OnTimeout(uint32_t nSockIndex, void* lpArg, uint32_t nTime);
public:
	ITcpServer* m_lpTcpServer;
};





#endif /* SERVICE_SAMPLE_H_ */
