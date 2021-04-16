#include <iostream>
#include "Address.h"

int main()
{
	//JINFENG::IPv4Address addr("192.168.0.1");
	JINFENG::IPv4Address::ptr addr = JINFENG::IPv4Address::Create("192.168.0.1", 8888);
	std::cout<<addr->getPort()<<std::endl;

	addr->setPort(1234);

	std::cout<<addr->getPort()<<std::endl;

	std::cout<<addr->getIp()<<std::endl;
}
