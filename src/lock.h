#pragma once

#include <pthread.h>
#include <assert.h>

namespace JINFENG{

class MutexLock{
public:
	MutexLock(){
		int ret = pthread_mutex_init(&m_mutex, NULL);
		assert(ret==0);
	}

	void lock()
	{
		int ret = pthread_mutex_lock(&m_mutex);
		assert(ret==0);
	}

	void unlock()
	{
		int ret = pthread_mutex_unlock(&m_mutex);
		assert(ret==0);
	}

	~MutexLock(){
		pthread_mutex_destroy(&m_mutex);
	}

	
private:
	pthread_mutex_t m_mutex;
};


class MutexLockGuard{
public:
	MutexLockGuard(MutexLock& mutex):m_mutex(mutex)
	{
		m_mutex.lock();
	}

	~MutexLockGuard()
	{
		m_mutex.unlock();
	}

private:
	MutexLock& m_mutex;
};







};
