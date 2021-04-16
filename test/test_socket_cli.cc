#include <iostream>
#include <assert.h>
#include "socket.h"

int main(){
	JINFENG::Socket::ptr clientfd = JINFENG::Socket::CreateTcpSocket();
	JINFENG::IPv4Address::ptr servaddr = JINFENG::IPv4Address::Create("127.0.0.1", 8888);
	/*  tcp  */
	
	bool b = clientfd->connect(servaddr);
	assert(b);
	std::cout<<"connect to server sucessfully.\n";

	char buf[20];
	clientfd->recv(buf, sizeof(buf), 0);

	std::cout<<"recv from server: "<<buf<<std::endl;
}
