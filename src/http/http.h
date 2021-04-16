#pragma once
#include<string>
#include<memory>
#include<string>
#include<string.h>
#include<map>
#include<iostream>
namespace JINFENG{

namespace HTTP{

//请求方法
#define HTTP_METHOD_MAP(xx) 	\
	xx(0, 	DELETE, DELETE) \
	xx(1, 	GET,   	GET) 	\
	xx(2, 	HEAD, 	HEAD) 	\
	xx(3, 	POST, 	POST)	\
	xx(4,	PUT, 	PUT)	


#define HTTP_STATUS_MAP(xx)	\
	xx(200, OK,		OK)	\
	xx(404, NOT_FOUND,	NOT_FOUND)


enum class HttpMethod{
#define xx(num, method, description) method = num,
	HTTP_METHOD_MAP(xx)
#undef xx
	INVALID_METHOD
};

enum class HttpStatus{
#define xx(code, name, description) name = code,
	HTTP_STATUS_MAP(xx)
#undef xx
};


HttpMethod StringToHttpMethod(std::string& m);

std::string HttpMethodToString(HttpMethod m);

HttpStatus StringToHttpStatus(std::string& s);

std::string HttpStatusToString(HttpStatus s);

int HttpStatusToCode(HttpStatus& s);

class HttpRequest{
public:
	typedef std::shared_ptr<HttpRequest> ptr;
	HttpRequest(uint8_t version=0x11, bool close=true);

	void setMethod(HttpMethod method)
	{
		m_method = method;
	}

	void setPath(std::string path)
	{
		m_path = path;
	}

	void setVersion(uint8_t version)
	{
		m_version = version;
	}

	void setHeader(std::string key, std::string value)
	{
		m_headers[key] = value;
	}
	
	void setHeaders(std::map<std::string, std::string> headers)
	{
		m_headers = headers;
	}

	void setBody(std::string body)
	{
		m_body = body;
	}

	std::ostream& dump(std::ostream& os);

private:
	HttpMethod m_method;
	std::string m_path;
	uint8_t m_version;
	bool m_close;
	std::map<std::string, std::string> m_headers;
	std::string m_body;
};

class HttpResponse{
public:
	typedef std::shared_ptr<HttpResponse> ptr;
	HttpResponse(uint8_t version=0x11, bool close=true);

	void setVersion(uint8_t version)
	{
		m_version = version;
	}

	void setStatus(HttpStatus status)
	{
		m_status = status;
	}

	void setHeader(std::string key, std::string value)
	{
		m_headers[key] = value;
	}

	void setHeaders(std::map<std::string, std::string> headers)
	{
		m_headers = headers;
	}

	void setBody(std::string body)
	{
		m_body = body;
	}

	std::ostream& dump(std::ostream& os);
private:
	uint8_t m_version;
	HttpStatus m_status;
	bool m_close;
	std::map<std::string, std::string> m_headers;
	std::string m_body;
};


}
};

