#ifndef THREAD_RECUR_MUTEX_H_H
#define THREAD_RECUR_MUTEX_H_H
#include "common.h"
class CThreadMutex{
public:
	CThreadMutex(){
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&_mutex,&attr);
		pthread_mutexattr_destroy(&attr);
	
	}
	~CThreadMutex(){
		pthread_mutex_destroy(&_mutex);
	}
	void lock(){
		pthread_mutex_lock(&_mutex);
	}
	int trylock(){
		return pthread_mutex_trylock(&_mutex);
	}
	void unlock(){
		pthread_mutex_unlock(&_mutex);
	}
private:
	pthread_mutex_t _mutex;
};
class CThreadGuard
{
public:
	CThreadGuard(CThreadMutex *mutex)
	{
		_mutex = NULL;
		if(mutex){
			_mutex=mutex;
			_mutex->lock();
		}
	}
		~CThreadGuard(){
			if(_mutex){
				_mutex->unlock();
			}
		}
	

private:
	CThreadMutex *_mutex;
};
#endif
