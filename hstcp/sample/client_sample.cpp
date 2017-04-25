#include "protocol.h"
#include "client_sample.h"
#include <string>
#include <iostream>
using namespace std;

CSampleClientBackCall::CSampleClientBackCall():pTcpClient(NULL)
{
	srand((unsigned)time(0));
}
CSampleClientBackCall::~CSampleClientBackCall()
{}
int CSampleClientBackCall::OnConnected()
{
	cout<<"in CSampleClientBackCall::OnConnected()"<<endl;
	
	char szBuf[sizeof(CLIENT_PACK_HEAD)+ sizeof(CLIENT_PACK_BODY)];
	memset(&szBuf,0,sizeof(CLIENT_PACK_HEAD)+ sizeof(CLIENT_PACK_BODY));

	((CLIENT_PACK_HEAD*)(((void*)(&szBuf))))->nLeng=sizeof(CLIENT_PACK_BODY);
	m_nRandomNum= rand() % 1000;
	((CLIENT_PACK_BODY*)((void*)(&szBuf)+sizeof(CLIENT_PACK_HEAD)))->nRandomNum = m_nRandomNum;

	pTcpClient->SendData((void*)(&szBuf),sizeof(CLIENT_PACK_HEAD)+sizeof(CLIENT_PACK_BODY));
	return 0;
}


int CSampleClientBackCall::OnDisconnect()
{
	cout<<"in CampleClientBackCall::OnDisconnect"<<endl;
	return 0;
}

int CSampleClientBackCall::OnRecvRawData(const char* lpData, uint32_t nLen)
{
	int nLeng = ((SERVER_PACK_HEAD*)lpData)->nLeng;
	return nLeng+sizeof(SERVER_PACK_HEAD);
}

int CSampleClientBackCall::OnRecvBiz(const char* lpBizData, uint32_t nBizLen)
{
	char szBuf[sizeof(CLIENT_PACK_HEAD)+ sizeof(CLIENT_PACK_BODY)];
	SERVER_PACK_BODY* pBody = (SERVER_PACK_BODY*)(lpBizData+sizeof(SERVER_PACK_HEAD));
	if(pBody->nAckNum == m_nRandomNum +1)
	{
		cout<<"successful translate number, the client is :"<<m_nRandomNum <<" the server is : "<< pBody->nAckNum<<endl;

		memset(&szBuf,0,sizeof(CLIENT_PACK_HEAD)+ sizeof(CLIENT_PACK_BODY));

		((CLIENT_PACK_HEAD*)(((void*)(&szBuf))))->nLeng=sizeof(CLIENT_PACK_BODY);
		m_nRandomNum= rand() % 1000;
		((CLIENT_PACK_BODY*)((void*)(&szBuf)+sizeof(CLIENT_PACK_HEAD)))->nRandomNum = m_nRandomNum;

		pTcpClient->SendData((void*)(&szBuf),sizeof(CLIENT_PACK_HEAD)+sizeof(CLIENT_PACK_BODY));
	}
	else
	{
		cout<<"fail to translate number ,the client is :"<<m_nRandomNum << " the server is : "<< pBody->nAckNum<<endl;
	}
	return 0;
}

int CSampleClientBackCall::OnError(int32_t iErrNo, const char* lpErrMsg)
{
	cout<<"in CSampleClientBackCall::OnError"<<endl;
	return 0;
}

int CSampleClientBackCall::OnHeartbeat(uint32_t nTime)
{
	cout<<"in CSampleClientBackCall::OnHeartbeat"<<endl;
	return 0;
}

int CSampleClientBackCall::OnTimeout(uint32_t nTime)
{
	cout<<"in CSampleClientBackCall::OnTimeout"<<endl;
	return 0;
}
bool g_bStart=false;
void sigint_handler(int sig)
{
	switch(sig)
	{
		case SIGINT:
		case SIGTERM:
			g_nStart=false;
			break;
		default:
			break;
	}
	
}
int main(int argc,char* argv[])
{
	ITcpClientCreator *pTcpClientCreator = GetClientCreator();
	if(pTcpCleintCreator == NULL)
	{
		cout<<"fail to get client creator"<<endl;
		exit(1);
	}
	if(pTcpClientCreator->Init(4))
	{
		cout<<"fail to init tcp client creator"<<endl;
		exit(1);
	}

	CSampleClientBackCall* pSampleClientBackCall = new CSampleClientBackCall;


	pTcpClientCreator->CreateTcpClient(&(pSampleClientBackCall->pTcpClient),"127.0.0.1",10000,pSampleClientBackCall);

	if(pSampleClientBackCall->pTcpClient == NULL)
	{
		cout<<"fail to createTcpClient"<<endl;
	}

	int ret =pSampleClientBackCall->pTcpClient->Connect();

	if(ret)
	{
		cout<<"fail to connect"<<endl;
	}
	g_bStart = true;
	while(g_bStart) //
	{
		sleep(1);
	}
	DestroyClientCreator(pTcpClientCreator);

}
