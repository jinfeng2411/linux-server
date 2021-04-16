#pragma once

#include "http.h"
#include <memory>
#include "Socket.h"

namespace JINFENG{

namespace http{


class HttpSession{
public:
	typedef std::shared_ptr<HttpSession> ptr;
	
	HttpSession(Socket::ptr sock, bool owner=true);

	HttpRequest::ptr recvRequest();

	int sendResponse(HttpResponse::ptr rsp);	

private:
	Socket::ptr m_socket;
};

};

};
