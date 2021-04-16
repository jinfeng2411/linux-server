#include "log.h"
#include <tuple>
#include <assert.h>
#include <iostream>

namespace JINFENG{

#define ISALPHA(ch) ((ch>='a' && ch<='z') || (ch>='A' && ch<='Z'))


/****************************** LogLevel ***************/

std::string LogLevel::ToString(LogLevel::Level level)
{
#define XX(name) \
	case LogLevel::name: return #name; break
	switch(level){
		XX(DEBUG);
		XX(INFO);
		XX(WARN);
		XX(ERROR);
		XX(FATAL);
		default: return "UNKNOW"; break;
	}
#undef XX
}


LogLevel::Level LogLevel::FromString(std::string name)
{
#define XX(name, str) \
	if(name == #str) return LogLevel::str;

	XX(name, DEBUG);
	XX(name, INFO);
	XX(name, WARN);
	XX(name, ERROR);
	XX(name, FATAL);

	return LogLevel::UNKNOW;	
}
#undef XX

/*************************** LogEvent ********************/

LogEvent::~LogEvent()
{
}

LogEventWrap::~LogEventWrap()
{
	m_event->getLogger()->log(m_event);
}

/*************************** LogFormatter ******************/

class StringFormatItem:public LogFormatter::FormatterItem{
public:
	StringFormatItem(std::string format){m_format = format;}
	void log(std::ostream& os, LogEvent::ptr event, LogLevel::Level level) override
	{
		os<<getFormat();
	}
};

class MessageFormatItem:public LogFormatter::FormatterItem{
public:
	MessageFormatItem(std::string format){m_format = format;}
        void log(std::ostream& os, LogEvent::ptr event, LogLevel::Level level) override
        {
                os<<event->getSS().str();
        }
};

class LevelFormatItem:public LogFormatter::FormatterItem{
public:
	LevelFormatItem(std::string format){m_format = format;}
        void log(std::ostream& os, LogEvent::ptr event, LogLevel::Level level) override
        {
                os<<LogLevel::ToString(event->getLevel());
        }
};

class ElapseFormatItem:public LogFormatter::FormatterItem{
public:

	ElapseFormatItem(std::string format){m_format = format;}
        void log(std::ostream& os, LogEvent::ptr event, LogLevel::Level level) override
        {
                os<<event->getEplase();
        }
};

class NameFormatItem:public LogFormatter::FormatterItem{
public:
	NameFormatItem(std::string format){m_format = format;}
        void log(std::ostream& os, LogEvent::ptr event, LogLevel::Level level) override
        {
                os<<event->getLogger()->getName();
        }
};

class ThreadIdFormatItem:public LogFormatter::FormatterItem{
public:
	ThreadIdFormatItem(std::string format){m_format = format;}
        void log(std::ostream& os, LogEvent::ptr event, LogLevel::Level level) override
        {
                os<<event->getThreadId();
        }
};

class NewLineFormatItem:public LogFormatter::FormatterItem{
public:
	NewLineFormatItem(std::string format){m_format = format;}
        void log(std::ostream& os, LogEvent::ptr event, LogLevel::Level level) override
        {
                os<<"\n";
        }
};

class DateTimeFormatItem:public LogFormatter::FormatterItem{
public:
	DateTimeFormatItem(std::string format){m_format=format;}
        void log(std::ostream& os, LogEvent::ptr event, LogLevel::Level level) override
        {
                struct tm tm;
		time_t time = event->getDate();
		localtime_r(&time, &tm);
		char buf[64];
		strftime(buf, sizeof(buf), getFormat().c_str(), &tm);
		os<<buf;
        }
};

class FileNameFormatItem:public LogFormatter::FormatterItem{
public:
	FileNameFormatItem(std::string format){m_format = format;}
        void log(std::ostream& os, LogEvent::ptr event, LogLevel::Level level) override
        {
                os<<event->getFileName();
        }
};

class LineFormatItem:public LogFormatter::FormatterItem{
public:
	LineFormatItem(std::string format){m_format = format;}
        void log(std::ostream& os, LogEvent::ptr event, LogLevel::Level level) override
        {
                os<<event->getLine();
        }
};

class TabFormatItem:public LogFormatter::FormatterItem{
public:
	TabFormatItem(std::string format){m_format=format;}
        void log(std::ostream& os, LogEvent::ptr event, LogLevel::Level level) override
        {
                os<<"\t";
        }
};

class FiberIdFormatItem:public LogFormatter::FormatterItem{
public:
	FiberIdFormatItem(std::string format){m_format=format;}
        void log(std::ostream& os, LogEvent::ptr event, LogLevel::Level level) override
        {
                os<<event->getFiber();
        }
};

class ThreadNameFormatItem:public LogFormatter::FormatterItem{
public:
	ThreadNameFormatItem(std::string format){m_format=format;}
        void log(std::ostream& os, LogEvent::ptr event, LogLevel::Level level) override
        {
                os<<event->getThreadName();
        }
};


void LogFormatter::init()
{	//keyword format flag
	std::vector<std::tuple<std::string, std::string, int>> items;

	std::string plainStr;
	for(int i=0;i<m_pattern.size();i++)
	{
		//std::cout<<"first: "<<m_pattern[i]<<std::endl;
		if(m_pattern[i]!='%')
		{
			plainStr += m_pattern[i];
			continue;
		}
		//detected a '%'
		//check the next char 
		if(m_pattern[i+1] == '%')  //next is '%'
		{
			plainStr += '%';
			++i;
			continue;
		}
		//next is keyword
		//if(!plainStr.empty())
		//	std::cout<<"plainStr: "<<plainStr<<std::endl;
		int n = i+1;
		std::string keyword = "";
		std::string format = "";
		bool startformat = false;
		int indformat = 0;
		while(n<m_pattern.size())
		{
			if(startformat==false && m_pattern[n] == '{')
			{
				keyword = m_pattern.substr(i+1, n-i-1);
				//std::cout<<"keyword: "<<keyword<<std::endl;
				startformat = true;
				++n;
				indformat = n;
				continue;
			}else if(startformat==true && m_pattern[n] == '}')
			{
				format = m_pattern.substr(indformat, n-indformat);
				//std::cout<<"format: "<<format<<std::endl;
				startformat = false;
				i = n;		
				break;
			}
			else if(startformat==false && !ISALPHA(m_pattern[n]))
			{
				if(keyword.empty())
					keyword = m_pattern.substr(i+1, n-i-1);
				//std::cout<<"keyword: "<<keyword<<std::endl;
				i = n-1;
				break;
			}
			++n;
			if(n==m_pattern.size())
			{
				keyword = m_pattern.substr(i+1, n-i-1);
				i = m_pattern.size();
				break;
			}

		}
		if(startformat)
		{
			assert(0);
		}
		if(!plainStr.empty())
		{
			items.push_back(std::make_tuple(plainStr, "", 0));
		}
		plainStr = "";
		items.push_back(std::make_tuple(keyword, format, 1));

		//
	}

	/*
	for(auto item: items)
        {
        	std::cout<<std::get<0>(item)<<" "<<std::get<1>(item)<<" "<<std::get<2>(item)<<std::endl;
        }
	*/
	for(auto item: items)
	{
		if(std::get<2>(item)==0)
		{
			//std::cout<<std::get<0>(item)<<std::endl;
			m_items.push_back(FormatterItem::ptr(new StringFormatItem(std::get<0>(item)))); //plain string
		}
		else{
			//m_items.push_back(FormatterItem::ptr(new MessageFormatItem("11")));
			
#define XX(type, II) \
	if(std::get<0>(item) == #type) \
		m_items.push_back(FormatterItem::ptr(new II(std::get<1>(item))))
		//m_items.push_back(FormatterItem::ptr(new ItemFunction(std::get<1>(item))))
			XX(m, MessageFormatItem);           //m:消息
       			XX(p, LevelFormatItem);            //p:日志级别
        		XX(r, ElapseFormatItem);            //r:累计毫秒数
        		XX(c, NameFormatItem);              //c:日志名称
        		XX(t, ThreadIdFormatItem);          //t:线程id
        		XX(n, NewLineFormatItem);           //n:换行
        		XX(d, DateTimeFormatItem);          //d:时间
        		XX(f, FileNameFormatItem);          //f:文件名
        		XX(l, LineFormatItem);              //l:行号
        		XX(T, TabFormatItem);               //T:Tab
        		XX(F, FiberIdFormatItem);           //F:协程id
        		XX(N, ThreadNameFormatItem);        //N:线程名称
#undef XX

		}
	}
	//std::cout<<m_items.size()<<std::endl;
}



void LogFormatter::log(std::ostream& os, LogEvent::ptr event)
{
	//std::cout<<"LogFormatter\n";
	//std::cout<<m_pattern<<std::endl;
	//std::cout<<m_items.size()<<std::endl;
	
	for(auto item: m_items)
	{
		item->log(os, event, JINFENG::LogLevel::ERROR);
		//std::cout<<"1\n";
	}
		
}	


/******************** FileAppender *************/
void FileLogAppender::log(LogEvent::ptr event)
{
	MutexLockGuard lock(m_mutex);
	reopen();
	m_formatter->log(m_os, event);
}

void FileLogAppender::reopen()
{
	if(m_os)
	{
		m_os.close();
	}
	m_os.open(m_filename);
}


/****************** StdAppender ****************/
void StdLogAppender::log(LogEvent::ptr event)
{
	MutexLockGuard lock(m_mutex);
	//std::cout<<"StdLogAppender\n";
	m_formatter->log(std::cout, event);	
}




/********************* Logger ******************/
void Logger::log(LogEvent::ptr event)
{
	//std::cout<<"log_1\n";
	for(auto appender: m_appenders)
	{
		//std::cout<<"log\n";
		appender->log(event);
	}
}

void Logger::addAppender(LogAppender::ptr appender)
{
	if(!appender->getFormatter())
		appender->setFormatter(m_formatter);
	m_appenders.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender)
{

}

void Logger::setFormatter(LogFormatter::ptr& formatter)
{
	//std::cout<<"setFormatter\n";
	m_formatter = formatter;
	//std::cout<<m_formatter->getPattern()<<std::endl;
}

LogFormatter::ptr Logger::getFormatter()
{
	return m_formatter;
}


/*********************** LoggerManager  ******************/

LoggerManager* LoggerManager::lmgr = nullptr;

LoggerManager::LoggerManager()
{	
	m_rootLogger = std::make_shared<Logger>(Logger("root"));

	JINFENG::LogFormatter::ptr formatter(new JINFENG::LogFormatter());
        m_rootLogger->setFormatter(formatter);

	LogAppender::ptr std(new StdLogAppender());
	m_rootLogger->addAppender(std);

	m_loggers["root"] = m_rootLogger;	
}


LoggerManager* LoggerManager::getInstance()
{
	if(lmgr==nullptr)
	{
		lmgr = new LoggerManager();
	}
	return lmgr;
}

Logger::ptr LoggerManager::getRootLogger()
{
	return m_rootLogger;
}


Logger::ptr LoggerManager::getLogger(std::string name)
{
	//若不存在名为 name 的 logger， 则新建一个并保存；若存在则直接返回
	if(m_loggers.find(name) == m_loggers.end())
	{
		Logger::ptr logger(new Logger(name));

	        JINFENG::LogFormatter::ptr formatter(new JINFENG::LogFormatter());
       		logger->setFormatter(formatter);

		LogAppender::ptr std(new StdLogAppender());
		logger->addAppender(std);

		m_loggers[name] = logger;

	}
	return m_loggers[name];
}

};
