/*
 * client_sampe.h
 *
 *  Created on: Apr 20, 2017
 *      Author: hlf
 */

#ifndef CLIENT_SAMPLE_H_
#define CLIENT_SAMPLE_H_
#include <tcp_client_interface.h>
class CSampleClientBackCall :public ITcpClientCallBack
{
public:
	CSampleClientBackCall();
	~CSampleClientBackCall();
	virtual int OnConnected();

	virtual int OnDisconnect();

	virtual int OnRecvRawData(const char* lpData, uint32_t nLen);

	virtual int OnRecvBiz(const char* lpBizData, uint32_t nBizLen);

	virtual int OnError(int32_t iErrNo, const char* lpErrMsg);

	virtual int OnHeartbeat(uint32_t nTime);

	virtual int OnTimeout(uint32_t nTime);
public:
	ITcpClient* pTcpClient;
	int m_nRandomNum;

};



#endif /* CLIENT_SAMPE_H_ */
