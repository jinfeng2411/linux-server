#pragma once

#include<string>
#include<memory>
#include<functional>
#include<pthread.h>
#include<assert.h>
#include "utils.h"

namespace JINFENG{

class Thread{
public:
	typedef std::shared_ptr<Thread> ptr;
	Thread(std::string name, std::function<void()> cb):m_name(name), m_cb(cb)
	{
		int ret = pthread_create(&t, nullptr, run, this);
		assert(ret==0);
	}

	void join()
	{
		int ret = pthread_join(t, nullptr);
		assert(ret==0);
	}

	std::string getThreadName() {return m_name;}

private:
	static void* run (void* args);

private:
	pthread_t t;
	std::string m_name;
	std::function<void()> m_cb;
};



};
