#include "service_sample.h"
#include "protocol.h"
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <string>
using namespace std;
int CSampleServerCallBack::Init()
{
	m_lpTcpServer=NULL;
	return 0;
}
int CSampleServerCallBack::OnAccepted(uint32_t nSockIndex,void**lppArg,const char* lpIPAddr,uint16_t wPort)
{
    cout<<"CSampleServerCallBack::OnAccepted"<<endl;
	return 0;
}

int CSampleServerCallBack::OnDisconnect(uint32_t nSockIndex, void* lpArg)
{
	cout<<"CSampleServerCallBack::OnDisconnect"<<endl;
	return 0;
}

int CSampleServerCallBack::OnRecvRawData(uint32_t nSockIndex,void* lpArg,const char* lpData,uint32_t nLen)
{
	int nl = ((CLIENT_PACK_HEAD*)lpData)->nLeng + sizeof(CLIENT_PACK_HEAD);
	return nl;
}

int CSampleServerCallBack::OnRecvBiz(uint32_t nSockIndex,void* lpArg,const char* lpBizData,uint32_t nBizLen)
{
	int nLen = ((CLIENT_PACK_BODY*)(lpBizData+sizeof(CLIENT_PACK_HEAD)))->nRandomNum;
	cout<<"accept RandomNum is :["<<nLen<<"]"<<endl;
	char szBuf[sizeof(SERVER_PACK_HEAD)+sizeof(SERVER_PACK_BODY)];
	((SERVER_PACK_HEAD*)((void*)&szBuf))->nLeng = sizeof(SERVER_PACK_BODY);
	((SERVER_PACK_BODY*)(((void*)&szBuf)+sizeof(SERVER_PACK_HEAD)))->nAckNum= nLen+1;

	m_lpTcpServer->SendData(nSockIndex,&szBuf,sizeof(SERVER_PACK_HEAD)+sizeof(SERVER_PACK_BODY));

	return 0;
}

int CSampleServerCallBack::OnError(int32_t iErrNo,uint32_t nSockIndex,void* lpArg,const char* lpErrMsg)
{
	cout<<"CSampleServerCallBack::OnError"<<endl;
	return 0;
}

int CSampleServerCallBack::OnHeartbeat(uint32_t nSockIndex, void* lpArg, uint32_t nTime)
{
	cout<<"CSampleServerCallBack::OnHeartbeat"<<endl;
	return 0;
}

int CSampleServerCallBack::OnTimeout(uint32_t nSockIndex, void* lpArg, uint32_t nTime)
{
	cout<<"CSampleServerCallBack::OnTimeout"<<endl;
	return 0;
}
bool g_bStart = false;
void sigint_handler(int sig)
{
	switch(sig)
	{
		case SIGINT:
			g_bStart = false;
			break;
		default:
			break;
	}
}


int main(int argc,char* argv[])
{
	signal(SIGINT,sigint_handler);
	CSampleServerCallBack *pSampleServerCallBack = new CSampleServerCallBack();
	if(pSampleServerCallBack == NULL)
	{
		return 1;
	}
	int ret = CreateTcpServer(&(pSampleServerCallBack->m_lpTcpServer),"127.0.0.1",10000,pSampleServerCallBack);
	if(pSampleServerCallBack->m_lpTcpServer != NULL)
	{
		g_bStart = true;
		while(g_bStart)
		{
			sleep(1);
		}
	}
	DestroyTcpServer(pSampleServerCallBack->m_lpTcpServer);

	if(pSampleServerCallBack != NULL)
	{
		delete pSampleServerCallBack;
	}


}
