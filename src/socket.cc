#include "socket.h"
#include <assert.h>
#include <iostream>
namespace JINFENG{

Socket::Socket(int family, int type, int protocol):m_family(family), m_type(type),m_protocol(protocol) 
{
	m_sock = socket(family, type, protocol);
	assert(m_sock>=0);
}

Socket::ptr Socket::CreateTcpSocket()
{
	Socket::ptr p(new Socket(IPv4, TCP));
	return p;
}

Socket::ptr Socket::CreateUdpSocket()
{
	Socket::ptr p(new Socket(IPv4, UDP));
	return p;
}

bool Socket::bind(const IPv4Address::ptr addr)
{
	m_localAddress = addr;	
	int ret = ::bind(m_sock, addr->getAddr(), addr->getAddrLen());
	return ret>=0;
}

bool Socket::listen(int backlog)
{
	int ret = ::listen(m_sock, backlog);
	assert(ret>=0);
	return ret>=0;
}

Socket::ptr Socket::accept()
{
	Socket::ptr p(new Socket(m_family, m_type, m_protocol));
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	socklen_t len = sizeof(addr);
	int ret = ::accept(m_sock, (struct sockaddr*)&addr, &len);
	assert(ret>=0);
	p->m_sock = ret;
	p->m_localAddress = m_localAddress;
	p->m_remoteAddress = IPv4Address::ptr(new IPv4Address(addr));
	p->m_isConnected = true;
	return p;

}
bool Socket::connect(const IPv4Address::ptr p, uint64_t timeout_ms)
{
	std::cout<<m_sock<<std::endl;
	int ret = ::connect(m_sock, p->getAddr(), p->getAddrLen());
	assert(ret>=0);
	m_isConnected = true;
	return ret>=0;
}

bool Socket::close()
{
	int ret = ::close(m_sock);
	assert(ret>=0);
	return ret>=0;
}

int Socket::send(const void* buffer, size_t length, int flags)
{
	int ret = -1;
	if(isConnected()){
		ret = ::send(m_sock, buffer, length, flags);
		assert(ret>=0);
	}
	return ret;
}

int Socket::sendTo(const void* buffer, size_t length, const IPv4Address::ptr to, int flags)
{
	int ret = -1;
	ret = ::sendto(m_sock, buffer, length, flags, to->getAddr(), to->getAddrLen());
	assert(ret>=0);
	return ret;
}

int Socket::recv(void* buffer, size_t length, int flags)
{
	int ret = -1;
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	size_t len = sizeof(addr);
	if(isConnected())
	{
		ret = ::recv(m_sock, buffer, length, flags);
		assert(ret>=0);
	}
}

int Socket::recvFrom(void* buffer, size_t length, JINFENG::IPv4Address::ptr from, int flags)
{
	int ret = -1;
	m_remoteAddress = IPv4Address::ptr(new IPv4Address());
	socklen_t len = m_remoteAddress->getAddrLen();
	ret = ::recvfrom(m_sock, buffer, length, flags, m_remoteAddress->getAddr(), &(len));
	assert(ret>=0);
	return ret;
}

IPv4Address::ptr Socket::getRemoteAddress()
{
	return m_remoteAddress;
}

IPv4Address::ptr Socket::getLocalAddress()
{
	return m_localAddress;
}



};
