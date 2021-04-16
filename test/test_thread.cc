#include"Thread.h"
#include<iostream>
#include<vector>
#include<string>
#include"lock.h"
int count = 0;

JINFENG::MutexLock mutex;
void func()
{
	JINFENG::MutexLockGuard lock(mutex);
	count++;
	std::cout<<JINFENG::gettid()<<"\t"<<count<<std::endl;
}


int main()
{
	std::vector<JINFENG::Thread::ptr> threads;
	for(int i=0;i<10;i++)
	{
		JINFENG::Thread::ptr t(new JINFENG::Thread(std::to_string(i), func));
		threads.push_back(t);
	}

	for(int i=0;i<10;i++)
	{
		threads[i]->join();
	}
}

