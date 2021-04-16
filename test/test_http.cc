#include "log.h"
#include "http.h"
#include <sstream>
#include "tcp_server.h"

int main()
{

	JINFENG::HTTP::HttpRequest::ptr req(new JINFENG::HTTP::HttpRequest());
	req->setHeader("Host", "www.baidu.com");
	req->setBody("Hello Resquest");

	JINFENG::HTTP::HttpResponse::ptr res(new JINFENG::HTTP::HttpResponse());
	res->setStatus((JINFENG::HTTP::HttpStatus)200);
	res->setBody("Hello Response");

	std::ostringstream os;

	req->dump(std::cout)<<std::endl;

	res->dump(std::cout)<<std::endl;
}
