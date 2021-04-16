#include<iostream>
#include"socket.h"
#include<assert.h>

int main()
{
	JINFENG::Socket::ptr listenfd = JINFENG::Socket::CreateTcpSocket();
	JINFENG::IPv4Address::ptr servaddr = JINFENG::IPv4Address::Create("127.0.0.1", 9998);
	listenfd->bind(servaddr);
	listenfd->listen();

	JINFENG::Socket::ptr sockfd;
	std::cout<<"listenning~~, waitting for connecting.....\n";
	while(1){
		sockfd = listenfd->accept();
		std::cout<<sockfd->getRemoteAddress()->getIp()<<" : "<<sockfd->getRemoteAddress()->getPort()<<std::endl;
		char buf[]="hello, Im server.\n";
		sockfd->send(buf, sizeof(buf), 0);
	}

}
