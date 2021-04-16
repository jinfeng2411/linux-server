#pragma once
#include <sys/socket.h>
#include <memory>
#include <unistd.h>
#include "Address.h"
namespace JINFENG{

class Socket{
public:
	typedef std::shared_ptr<Socket> ptr;

	enum Type{
		TCP = SOCK_STREAM,
		UDP = SOCK_DGRAM
	};

	enum Family{
		IPv4 = AF_INET,
		IPv6 = AF_INET6
	};

	static Socket::ptr CreateTcpSocket();

	static Socket::ptr CreateUdpSocket();

	Socket(int family, int type, int protocol=0);

	
	bool bind(const IPv4Address::ptr addr);
	
	bool listen(int backlog = 5);
	
	Socket::ptr accept();

	bool connect(const IPv4Address::ptr p, uint64_t timeout_ms=-1);

	bool isConnected(){return m_isConnected;}

	bool close();

	int send(const void* buffer, size_t length, int flags);

	int sendTo(const void* buffer, size_t length, const IPv4Address::ptr to, int flags);

	int recv(void* buffer, size_t length, int flags = 0);

	int recvFrom(void* buffer, size_t length, JINFENG::IPv4Address::ptr from, int flags);
	IPv4Address::ptr getRemoteAddress();

	IPv4Address::ptr getLocalAddress();

private:
	int m_sock = -1;
	int m_family = -1;
	int m_type = -1;
	int m_protocol = -1;
	bool m_isConnected = false;
	IPv4Address::ptr m_localAddress = nullptr;
	IPv4Address::ptr m_remoteAddress = nullptr;
};

}
