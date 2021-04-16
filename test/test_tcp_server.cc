#include "tcp_server.h"
#include "iomanager.h"
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include "log.h"
int main()
{
	JINFENG::Logger::ptr root = JINFENG_LOG_ROOT;
	JINFENG_LOG_DEBUG(root)<<"hello";

	JINFENG::Logger::ptr system = JINFENG_LOG_NAME("system");
        JINFENG_LOG_DEBUG(system)<<"system";


	JINFENG::IOManager* io = new JINFENG::IOManager("iomanager", 3);
	
	JINFENG::TcpServer::ptr server(new JINFENG::TcpServer("jinfeng", io));

	JINFENG::IPv4Address::ptr addr = JINFENG::IPv4Address::Create("127.0.0.1", 8888);

	bool ret = server->bind(addr);
	assert(ret);
	
	sleep(1);
	ret = server->start();
	assert(ret);
	
	sleep(120);
}
