#pragma once
#include<arpa/inet.h>
#include<sys/socket.h>
#include<memory>
#include<string>
#include<string.h>

namespace JINFENG{


class IPv4Address{
public:
	typedef std::shared_ptr<IPv4Address> ptr;

	static IPv4Address::ptr Create(const char* address, uint16_t port = 0);

	IPv4Address(const sockaddr_in& address);

	IPv4Address(uint32_t address=INADDR_ANY, uint16_t port = 0);

	sockaddr* getAddr() const;

	socklen_t getAddrLen() const;

	uint16_t getPort() const;

	std::string getIp() const;

	void setPort(uint16_t port);
private:
	sockaddr_in m_addr;
};


};
