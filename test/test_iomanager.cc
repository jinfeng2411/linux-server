#include "iomanager.h"
#include <iostream>
#include <unistd.h>

void func()
{
	std::cout<<"in func.\n";
}

void timerFunc()
{
	std::cout<<"in timer func.\n";
}

int main()
{
	JINFENG::IOManager::ptr IOMgr(new JINFENG::IOManager());
	
	//IOMgr->addTimer(3000, timerFunc, true);
	IOMgr->schedule(func);	
	//
	sleep(10);
}
