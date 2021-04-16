#include "tcp_server.h"
#include <assert.h>
#include <iostream>
#include "log.h"
namespace JINFENG{

Logger::ptr system = JINFENG_LOG_NAME("system");

TcpServer::TcpServer(std::string name, IOManager* worker):m_name(name), m_worker(worker)
{
	m_isStop = true;
}

TcpServer::~TcpServer()
{
	for(auto& sock: m_socks)
		sock->close();
	m_socks.clear();
}

bool TcpServer::bind(IPv4Address::ptr addr)
{
	std::vector<IPv4Address::ptr> addrs;
	std::vector<IPv4Address::ptr> fails;
	addrs.push_back(addr);
	return bind(addrs, fails);
}

bool TcpServer::bind(const std::vector<IPv4Address::ptr>& addrs, std::vector<IPv4Address::ptr>& fails)
{
	for(auto& addr: addrs)
	{
		Socket::ptr sock = Socket::CreateTcpSocket();
		if(!sock->bind(addr))
		{
			fails.push_back(addr);
			continue;
		}
		if(!sock->listen())
		{
			assert(false);
			fails.push_back(addr);
			continue;
		}
		m_socks.push_back(sock);
	}
	if(!fails.empty()){
		m_socks.clear();
		return false;
	}
	return true;
}

bool TcpServer::start()
{
	if(!m_isStop)
		return true;
	m_isStop = false;
	for(auto& sock: m_socks)
	{
		m_worker->schedule(std::bind(&TcpServer::startAccept, shared_from_this(), sock));
	}
	return true;
}

bool TcpServer::stop()
{

}

void TcpServer::handleClient(Socket::ptr client)
{
	JINFENG_LOG_DEBUG(system)<<"in handleClient";
	char buf[] = "hello, I'm server.\n";
	client->send(buf, sizeof(buf), 0);
	JINFENG_LOG_DEBUG(system)<<"send \""<<buf<<"\" to "<<client->getRemoteAddress()->getIp()<<" : "<<client->getRemoteAddress()->getPort();
}

void TcpServer::startAccept(Socket::ptr sock)
{
	JINFENG_LOG_DEBUG(system)<<"waitting for connection on "<<sock->getLocalAddress()->getIp()<<" : "<<sock->getLocalAddress()->getPort();
	while(!m_isStop){
		Socket::ptr client = sock->accept();
		JINFENG_LOG_DEBUG(system)<<"accept connection from "<<client->getRemoteAddress()->getIp()<<" : "<<client->getRemoteAddress()->getPort();
		m_worker->schedule(std::bind(&TcpServer::handleClient, shared_from_this(), client));
		
	}
}

};
