#include "fiber.h"
#include <iostream>


void func()
{
	std::cout<<"in func\n";

	JINFENG::Fiber::YieldToHold();

	std::cout<<"in func\n";
	
//	JINFENG::Fiber::YieldToHold();
}


int main()
{
	JINFENG::Fiber::getThis();
	std::cout<<"in main\n";
	JINFENG::Fiber::ptr f1(new JINFENG::Fiber(func));
	f1->swapIn();
	std::cout<<"in main\n";
	f1->swapIn();
	std::cout<<"in main\n";

}
