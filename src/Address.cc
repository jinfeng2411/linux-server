#include "Address.h"

namespace JINFENG{


IPv4Address::ptr IPv4Address::Create(const char* address, uint16_t port)
{
	IPv4Address::ptr rt(new IPv4Address);
	rt->m_addr.sin_family = AF_INET;
	rt->m_addr.sin_port = htons(port);
	inet_pton(AF_INET, address, &(rt->m_addr.sin_addr));
	return rt;
}

IPv4Address::IPv4Address(const sockaddr_in& address)
{
	m_addr = address;
}


IPv4Address::IPv4Address(uint32_t address, uint16_t port)
{
	memset(&m_addr, 0, sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(port);
	m_addr.sin_addr.s_addr = address;
}


sockaddr* IPv4Address::getAddr() const
{
	return (sockaddr*)&m_addr;
}

socklen_t IPv4Address::getAddrLen() const
{
	return sizeof(m_addr);
}

uint16_t IPv4Address::getPort() const
{
	return ntohs(m_addr.sin_port);
}

std::string IPv4Address::getIp() const
{
	char buf[20];
	inet_ntop(AF_INET, &m_addr.sin_addr, buf, sizeof(buf));
	return std::string(buf);
}

void IPv4Address::setPort(uint16_t port)
{
	m_addr.sin_port = htons(port);
}


};
