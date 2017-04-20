#include "thread.h"
#include "time_tools.h"

void CThread::ExitThread(void* arg)
{
	if(arg)
	{
	//	printf("in CThread::ExitThread\n");
		CThread* pThread = (CThread*)arg;
		pthread_mutex_lock(&pThread->_condmutex);
		pThread->_bStarted = false;
		pthread_cond_signal(&pThread->_cond);
		pthread_mutex_unlock(&pThread->_condmutex);
	
	}
}
void* CThread::hook(void* arg)
{
	void* rc = NULL;
	if(arg)
	{
		CThread* pThread = (CThread*)arg;
		pthread_mutex_lock(&pThread->_condmutex);
		pThread->_bStarted = true;
		pthread_cleanup_push(CThread::ExitThread,pThread);
		pthread_cond_signal(&pThread->_cond);
		pthread_mutex_unlock(&pThread->_condmutex);

		rc = (void*)pThread->Run();

		pthread_cleanup_pop(1);
	}
	return rc;
}
CThread::CThread():_hThread(0),_bStarted(false),_bTerminate(false)
{}

CThread::~CThread()
{
	if(_bStarted)
	{
		stop(100);
	}
	pthread_mutex_destroy(&_condmutex);
	pthread_cond_destroy(&_cond);
}
int CThread::start()
{
	int ret = 0;
	ret = pthread_mutex_lock(&_condmutex);
	if(ret != 0)
	{
		return ret;
	}
	if(!_bStarted)
	{
		_bTerminate=false;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr,1024 * 1024 * 10);
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

		ret = pthread_create(&_hThread,&attr,CThread::hook,(void*)this);
		if(ret == 0)
		{
			waitCond();
		}
		pthread_attr_destroy(&attr);
	}
	ret = pthread_mutex_unlock(&_condmutex);
	return (int32_t)ret;
}
int CThread::stop(int iWaitTime)
{
	int rc = 0;
	rc = pthread_mutex_lock(&_condmutex);
	if(rc != 0)
	{
		return rc;
	}
	if(_bStarted && !pthread_equal(pthread_self(),_hThread))
	{
		Terminate();
		waitCond(iWaitTime);
		if(!_bStarted)
		{
			_bTerminate = false;
		}
		else
		{
			if(pthread_cancel(_hThread) == 0)
			{
				waitCond(10);
			}
			if(!_bStarted)
			{
				_bTerminate = false;
			}
			else
			{
				rc = -1;
			}
		}
	}
	rc = pthread_mutex_unlock(&_condmutex);
	return (int32_t)rc;
}
int CThread::waitCond(int iWaitTime)
{
	int rc = 0;
	if(iWaitTime < 0)
	{
		rc = pthread_cond_wait(&_cond,&_condmutex);	
	}
	else
	{
		timespec ts;
		getTimespec(iWaitTime,ts);
		rc = pthread_cond_timedwait(&_cond,&_condmutex,&ts);
	}
	return rc;
}
