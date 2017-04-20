#include <tcp_client_interface.h>
#include <string>
#include <iostream>
using namespace std;

int CSampleClient::Connect(const char* lpServerAddr,
		uint16_t wServerPort,
		bool bBlockConnect,
		const char* lpLocalAddr)
{
	return 0;
}

int CSampleClient::SendData(
							const void* lpData,
							int32_t iDataLen,
							int32_t iWaitTime,
							uint32_t nBlockFlag)
{
	return 0;
}

int CSampleClient::Disconnect()
{
	return 0;
}

int CSampleClient::ReConnect(
 							const char*	lpServerAddr,
 							uint16_t	wServerPort,
 							bool bBlockConnect,
 							const char* lpLocalAddr )
{
	return 0;
}

int CSampleClient::SetHeartbeat(int32_t iTime)
{
	return 0;
}

int CSampleClient::SetTimeout(int32_t iTime)
{
	return 0;
}

int CSampleClient::SetQuickAck(bool bQuickAck)
{
	return 0;
}

int CSampleClient::SetTcpNoDelay(bool bNoDelay)
{
	return 0;
}


int CSampleClient::SetTcpKeepAlive(bool bKeepAlive)
{
	return 0;
}


int CSampleClient::SetAutoReConnect(bool bAutoReConnect)
{
	return 0;
}


bool CSampleClient::GetAutoReConnect()
{
	return false;
}

bool CSampleClient::GetConnectStatus()
{
	return false;
}


int CSampleClient::GetConnectCount()
{
	return 0;
}

int CSampleClientBackCall::OnConnected()
{
	return 0;
}


int CSampleClientBackCall::OnDisconnect()
{
	return 0;
}

int CSampleClientBackCall::OnRecvRawData(const char* lpData, uint32_t nLen)
{
	return 0;
}

int CSampleClientBackCall::OnRecvBiz(const char* lpBizData, uint32_t nBizLen)
{
	return 0;
}

int CSampleClientBackCall::OnError(int32_t iErrNo, const char* lpErrMsg)
{
	return 0;
}

int CSampleClientBackCall::OnHeartbeat(uint32_t nTime)
{
	return 0;
}

int CSampleClientBackCall::OnTimeout(uint32_t nTime)
{
	return 0;
}

int main(int argc,char* argv[])
{

}
