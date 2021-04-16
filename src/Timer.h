#pragma once

#include<memory>
#include<string>
#include<set>
#include<functional>
#include"utils.h"
#include<stdint.h>
#include<limits.h>

namespace JINFENG{

class TimerManager;


class Timer{
friend class TimerManager;
public:
	typedef std::shared_ptr<Timer> ptr;

private:
	Timer(uint64_t ms, std::function<void()> cb, bool recurring, TimerManager* manager):m_ms(ms), m_cb(cb), m_recurring(recurring), m_manager(manager)
	{
		m_next = getTimeMs() + m_ms;
	}
private:
	uint64_t m_ms;
	uint64_t m_next;
	std::function<void()> m_cb;
	bool m_recurring;
	TimerManager* m_manager;
private:
	struct Comparator{
		bool operator()(const Timer::ptr& lhs, const Timer::ptr& rhs) const
		{
			if(!lhs && !rhs)
				return false;
			if(!lhs)
				return true;
			if(!rhs)
				return false;
			if(lhs->m_next<=rhs->m_next)
				return true;
			return false;
		}
	};
};


class TimerManager{
friend class Timer;
public:
	typedef std::shared_ptr<TimerManager> ptr;

	TimerManager(){
		last_time = JINFENG::getTimeMs();
	}

	virtual ~TimerManager()
	{
	}

	void addTimer(uint64_t ms, std::function<void()> cb, bool recurring=false)
	{
		Timer::ptr t(new Timer(ms, cb, recurring, this));
		m_set.insert(t);
	}

	uint64_t getNextTimer()
	{
		if(m_set.empty())
			return INT_MAX;
		return (*(m_set.begin()))->m_next - JINFENG::getTimeMs();
	}

	void handleTimer(uint64_t nowTime) //处理nowTime时间之前的定时器
	{
		auto begin = m_set.begin();
		for(begin; begin!=m_set.end(); begin++)
		{
			if((*begin)->m_next <= nowTime)
			{
				(*begin)->m_cb();
			}
		}
		m_set.erase(m_set.begin(), begin);
	}	

private:
	std::set<Timer::ptr, Timer::Comparator> m_set;
	uint64_t last_time;
};


};
