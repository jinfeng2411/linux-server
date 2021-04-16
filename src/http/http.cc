#include "http.h"

namespace JINFENG{

namespace HTTP{


HttpMethod StringToHttpMethod(std::string& m)
{
#define xx(num, method, description) \
        if(strncmp(#method, m.c_str(), strlen(#method))==0){ \
                return HttpMethod::method;\
        }
        HTTP_METHOD_MAP(xx)
#undef xx
        return HttpMethod::INVALID_METHOD;
}


std::string HttpMethodToString(HttpMethod m)
{
#define xx(num, method, description) \
        if(m==HttpMethod::method) \
                return std::string(#method);
        HTTP_METHOD_MAP(xx)
        return "<unknow>";
#undef xx
}

HttpStatus StringToHttpStatus(std::string& s)
{
#define xx(code, name, description) \
        if(strncmp(#name, s.c_str(), strlen(#name))==0) \
                return HttpStatus::name;
        HTTP_STATUS_MAP(xx)
#undef xx
}


std::string HttpStatusToString(HttpStatus s)
{
#define xx(code, name, description) \
        if(s==HttpStatus::name) \
                return #name;
        HTTP_STATUS_MAP(xx);
        return "<unknow>";
#undef xx
}

int HttpStatusToCode(HttpStatus& s)
{
#define xx(code, name, description) \
        if(s==HttpStatus::name) \
                return code;
        HTTP_STATUS_MAP(xx);
#undef xx
}


HttpRequest::HttpRequest(uint8_t version, bool close):m_version(version), m_close(close)
{
	m_method = HttpMethod::GET;
	m_path = "/";
}

std::ostream& HttpRequest::dump(std::ostream& os)
{
	os <<HttpMethodToString(m_method)<<" " 
	   <<m_path<<" "
	   <<"HTTP/"
	   <<(m_version>>4)
	   <<"."
	   <<(m_version&0x0f)
	   <<"\r\n";
	os <<"connection: "<<(m_close? "close":"keep-alive")<<"\r\n";
	for(auto& item: m_headers)
	{
		os<<item.first<<":"<<item.second<<"\r\n";
	}
	if(!m_body.empty())
	{
		os <<"content-length: "<<m_body.size()<<"\r\n\r\n"
			<<m_body;
	}else{
		os <<"\r\n";
	}
	return os;
}


HttpResponse::HttpResponse(uint8_t version, bool close):m_version(version), m_close(close)
{
	m_status = HttpStatus::OK;
}

std::ostream& HttpResponse::dump(std::ostream& os)
{
	os <<"HTTP/"
	   <<(m_version>>4)
	   <<"."
	   <<(m_version&0x0f)
	   <<" "
	   <<HttpStatusToCode(m_status)
	   <<" "
	   <<HttpStatusToString(m_status)
	   <<"\r\n";
	os <<"connection: "<<(m_close? "close":"keep_alive")<<"\r\n";
	for(auto& item: m_headers)
	{
		os<<item.first<<":"<<item.second<<"\r\n";
	}
	if(!m_body.empty())
	{
		os<<"content-length: "<<m_body.size()<<"\r\n\r\n"
			<<m_body;
	}
	else
	{
		os<<"\r\n";
	}
	return os;
}

};
};
