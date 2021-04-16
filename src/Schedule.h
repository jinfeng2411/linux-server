#pragma once
#include <pthread.h>
#include <vector>
#include <string>
#include <memory>
#include <list>
#include <functional>
#include <stdint.h>
#include "utils.h"
#include "Thread.h"
#include "fiber.h"
#include <iostream>
#include "lock.h"
#include "log.h"
namespace JINFENG{

//static Logger::ptr system = JINFENG_LOG_NAME("system");

class Schedule{
public:
	typedef std::shared_ptr<Schedule> ptr;

	struct FiberAndThread{
		typedef std::shared_ptr<FiberAndThread> ptr;
		uint64_t threadId = -1;
		JINFENG::Fiber::ptr fiber = nullptr;
		std::function<void()> cb = nullptr;

		FiberAndThread(uint64_t threadId, JINFENG::Fiber::ptr fiber):threadId(threadId), fiber(fiber){}

		FiberAndThread(uint64_t threadId, std::function<void()> cb):threadId(threadId), cb(cb){}
		FiberAndThread():threadId(-1){}

		void reset()
		{
			threadId = -1;
			fiber = nullptr;
			cb = nullptr;
		}
	};

	Schedule(std::string name="", size_t threads=1, bool use_caller=false);

	virtual ~Schedule();

	template<class FiberOrCb>
	bool schedule(FiberOrCb fc, int thread = -1)
	{
        	//JINFENG_LOG_DEBUG(system)<<"in schedule";

        	bool need_tickle = m_fibers.empty();
        	FiberAndThread::ptr f(new FiberAndThread(thread, fc));
        	m_fibers.push_back(f);

        	if(need_tickle){
                	//JINFENG_LOG_DEBUG(system)<<"need tickle";
                	tickle();
        	}
	}

	void start();

	virtual void stop();

	static Schedule* getThis();

protected:
	void setThis();

	virtual void tickle();

	void run();

	virtual void idle();

	JINFENG::MutexLock m_mutex;

private:
	bool m_stoping = true;
	std::string m_name;
	size_t m_threadsNum;
	std::vector<JINFENG::Thread::ptr> m_threads;
	std::list<FiberAndThread::ptr> m_fibers;
};

}
