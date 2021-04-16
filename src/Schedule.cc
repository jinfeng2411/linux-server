#include "Schedule.h"
#include "lock.h"
#include <iostream>
#include "log.h"

namespace JINFENG{

static thread_local Schedule* t_schedule = nullptr;

static JINFENG::Logger::ptr system = JINFENG_LOG_NAME("system");
   
Schedule::Schedule(std::string name, size_t threads, bool use_caller)
{
	m_name = name;
	m_threadsNum = threads;
}

Schedule::~Schedule(){
	for(auto t: m_threads)
	{
		t->join();
	}
}

Schedule* Schedule::getThis()
{
	return t_schedule;
}

void Schedule::setThis()
{
	t_schedule = this;
}


void Schedule::start()
{
	JINFENG_LOG_DEBUG(system)<<"begin Schedule::start";
	for(int i=0; i<m_threadsNum; i++)
	{
		Thread::ptr t(new Thread(std::to_string(i), std::bind(&Schedule::run,this)));
		m_threads.push_back(t);
	}
}

void Schedule::stop()
{
        for(auto t: m_threads)
        {
                t->join();
        }
	
}

void Schedule::tickle()
{
	JINFENG_LOG_DEBUG(system)<<"tickle";
}

void Schedule::run()
{
	JINFENG_LOG_DEBUG(system)<<"begin Schedule::run";
	Fiber::getThis(); //否则后面 idle_fiber->swapIn() 会出错，因为 main_fiber 没有准备好
	Fiber::ptr idle_fiber(new Fiber(std::bind(&Schedule::idle,this)));
	setThis();
	while(1){
		bool need_tickle = false;
		bool get_task = false;
		FiberAndThread::ptr task;
		{
		JINFENG::MutexLockGuard lock(m_mutex);
		for(auto begin = m_fibers.begin(); begin!=m_fibers.end(); begin++)
		{
			if((*begin)->threadId!=-1 && (*begin)->threadId!=gettid())
			{
				need_tickle = true;
				continue;
			}
			task = m_fibers.front();
			m_fibers.pop_front();
			get_task = true;
			break;
		}
		}
		if(need_tickle)
			tickle();
	
		if(get_task)
		{
			assert(task->fiber!=nullptr || task->cb!=nullptr);
			if(task->fiber)
			{
				task->fiber->swapIn();
			}
			if(task->cb)
			{
				task->cb();
			}
			task->reset();
			continue;
		}
		JINFENG_LOG_DEBUG(system)<<"before swapIn";
		
		idle_fiber->swapIn();
	}
}

void Schedule::idle()
{
	JINFENG_LOG_DEBUG(system)<<"Schedule::idle";
}
	


};
