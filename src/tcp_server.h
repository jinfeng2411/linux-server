#pragma once
#include<string>
#include<memory>
#include<vector>
#include "Address.h"
#include "socket.h"
#include "iomanager.h"

namespace JINFENG{

class TcpServer: public std::enable_shared_from_this<TcpServer>{
public:
	typedef std::shared_ptr<TcpServer> ptr;
	TcpServer(std::string name, IOManager* worker);
	virtual ~TcpServer();

	virtual bool bind(IPv4Address::ptr addr);

	virtual bool bind(const std::vector<IPv4Address::ptr>& addrs, std::vector<IPv4Address::ptr>& fails);

	virtual bool start();

	virtual bool stop();

	bool isStop(){return m_isStop;}

	std::string getName(){ return m_name;}

protected:
	virtual void handleClient(Socket::ptr client);

	virtual void startAccept(Socket::ptr sock);


private:
	std::string m_name;
	std::vector<Socket::ptr> m_socks;
	IOManager* m_worker;
	bool m_isStop;
};



};
