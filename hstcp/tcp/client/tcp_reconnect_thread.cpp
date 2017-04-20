#include "tcp_reconnect_thread.h"
#include <base/tcp_lib_attr.h>
#include "tcp_client_creator.h"
extern CTcpClientCreator*	glpTcpClientCreator;
long CReConnectThread::Run()
{
	uint32_t nCount=0;
	THREAD_SIGNAL_BLOCK
	while(!isTerminated())
	{
		//sleep10毫秒
		usleep(10000);
		++nCount;
		//每隔0.1秒，检查一次
		if( (nCount%10) == 0 )
		{
			glpTcpClientCreator->CheckConnection();
		}
	}
	return 0;
}
