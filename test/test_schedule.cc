#include "Schedule.h"
#include <iostream>
#include "log.h"
#include <unistd.h>
#include "lock.h"

JINFENG::MutexLock mutex; 

static JINFENG::Logger::ptr logger = JINFENG_LOG_ROOT;

void func()
{
	JINFENG::MutexLockGuard lock(mutex);
	static int count = 10;

	JINFENG_LOG_DEBUG(logger)<<"in func";

	sleep(1);

	while(count-->0)
	{
		JINFENG::Schedule::getThis()->schedule(func, -1);
	}
}


int main()
{

	JINFENG::Schedule::ptr p(new JINFENG::Schedule("jinfeng-schedule", 5));

	bool ret = p->schedule(func,-1);

//	ret = p->schedule(func,-1);

//	ret = p->schedule(func, -1);

	p->start();

	p->stop();
}
