#include "service_sample.h"
#include <iostream>
#include <string>
using namespace std;
int CSampleServerCallBack::OnAccepted(uint32_t nSockIndex,void**lppArg,const char* lpIPAddr,uint16_t wPort)
{
	return 0;
}

int CSampleServerCallBack::OnDisconnect(uint32_t nSockIndex, void* lpArg)
{
	return 0;
}

int CSampleServerCallBack::OnRecvRawData(uint32_t nSockIndex,void* lpArg,const char* lpData,uint32_t nLen)
{
	return 0;
}

int CSampleServerCallBack::OnRecvBiz(uint32_t nSockIndex,void* lpArg,const char* lpBizData,uint32_t nBizLen)
{
	return 0;
}

int CSampleServerCallBack::OnError(int32_t iErrNo,uint32_t nSockIndex,void* lpArg,const char* lpErrMsg)
{
	return 0;
}

int CSampleServerCallBack::OnHeartbeat(uint32_t nSockIndex, void* lpArg, uint32_t nTime)
{
	return 0;
}

int CSampleServerCallBack::OnTimeout(uint32_t nSockIndex, void* lpArg, uint32_t nTime)
{
	return 0;
}


int CSampleServer::SendData(uint32_t nSockIndex,const void* lpData,int32_t iDataLen,int32_t iWaitTime,uint32_t nBlockFlag)
{}

int CSampleServer::Disconnect(uint32_t nSockIndex)
{}

int CSampleServer::SetHeartbeat(uint32_t nSockIndex,int32_t iTime)
{}

int CSampleServer::SetTimeout(uint32_t nSockIndex, int32_t iTime)
{}

int CSampleServer::SetQuickAck(uint32_t nSockIndex, bool bQuickAck)
{}

int CSampleServer::SetTcpNoDelay(uint32_t nSockIndex, bool bNoDelay)
{}

int CSampleServer::SetTcpKeepAlive(uint32_t nSockIndex, bool bKeepAlive)
{}

int CSampleServer::GetSendDataTimes()
{}
int main(int argc,char* argv[])
{


}
