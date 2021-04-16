#include "utils.h"
#include <iostream>
#include "Timer.h"
using namespace std;

void func()
{

}

int main()
{
	JINFENG::TimerManager::ptr tmgr(new JINFENG::TimerManager());
	tmgr->addTimer(500, func, false);

	cout<<tmgr->getNextTimer()<<endl;
}
