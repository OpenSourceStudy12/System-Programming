#ifndef MUTEX_H
#define MUTEX_H

#include "noncpyable.h"
#include <pthread.h>

class Mutex : noncpyable {
public:
	Mutex();
	~Mutex();

	void lock();
	void unlock();
	
	pthread_mutex_t* getPthreadMutex()
	{
		return &mutex_;
	}
private:
	pthread_mutex_t mutex_;
};

class LockGuard : noncpyable {
public:
	LockGuard(Mutex& mutex) : mutex_(mutex)
	{
		mutex_.lock();
	}
	~LockGuard()
	{
		mutex_.unlock();
	}
private:
	Mutex& mutex_;
};

#endif




