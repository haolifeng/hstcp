#ifndef THREAD_H_H
#define THREAD_H_H
#include "common.h"
class CThread{
public:
	CThread();
	virtual ~CThread();
	int start();
	int stop(int nWaitTime=-1);
	bool isRunning()const
	{
		return _bStarted;
	}
	bool isTerminated() const
	{
		return _bTerminate;
	}
	void Terminate()
	{
		_bTerminate = true;
	}
	static void *hook(void* arg);
	static void ExitThread(void* arg);
protected:
	virtual long Run() = 0;
private:
	CThread(const CThread& );
	CThread& operator=(const CThread&);
	int waitCond(int nWaitTime=-1);
private:
	pthread_mutex_t _condmutex;
	pthread_cond_t _cond;
	pthread_t _hThread;
	volatile bool _bStarted;
	volatile bool _bTerminate;



};
#endif
