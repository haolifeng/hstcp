#ifndef TCP_RECONNECT_THREAD_H
#define TCP_RECONNECT_THREAD_H
#include <sys/thread.h>
class CReConnectThread:public CThread
{
public:
	virtual long Run();
};
#endif
