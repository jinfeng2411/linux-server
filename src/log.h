#pragma once

#include <memory>
#include <string>
#include <stdint.h>
#include <sstream>
#include <vector>
#include <time.h>
#include <iostream>
#include <fstream>
#include "utils.h"
#include <map>
#include "lock.h"

namespace JINFENG{

#define JINFENG_LOG_ROOT JINFENG::LoggerManager::getInstance()->getRootLogger()
#define JINFENG_LOG_NAME(name) JINFENG::LoggerManager::getInstance()->getLogger(name)


#define JINFENG_LOG_LEVEL(logger, level) \
	JINFENG::LogEventWrap(JINFENG::LogEvent::ptr(new JINFENG::LogEvent(logger, __FILE__, 0, __LINE__, "threadName", JINFENG::gettid(), 0, time(0), level))).getSS()


#define JINFENG_LOG_DEBUG(logger) JINFENG_LOG_LEVEL(logger, JINFENG::LogLevel::DEBUG)
#define JINFENG_LOG_INFO(logger) JINFENG_LOG_LEVEL(logger, JINFENG::LogLevel::INFO)
#define JINFENG_LOG_WARN(logger) JINFENG_LOG_LEVEL(logger, JINFENG::LogLevel::WARN)
#define JINFENG_LOG_ERROR(logger) JINFENG_LOG_LEVEL(logger, JINFENG::LogLevel::ERROR)
#define JINFENG_LOG_FATAL(logger) JINFENG_LOG_LEVEL(logger, JINFENG::LogLevel::FATAL)
#define JINFENG_LOG_UNKNOW(logger) JINFENG_LOG_LEVEL(logger, JINFENG::LogLevel::UNKNOW)

class LogLevel{
public:
	enum Level{
		UNKNOW = 0,
		DEBUG = 1,
		INFO = 2,
		WARN = 3,
		ERROR = 4,
		FATAL = 5
	};

	static std::string ToString(LogLevel::Level level);

	static LogLevel::Level FromString(std::string);
};


class Logger;

class LogEvent{
public:
	typedef std::shared_ptr<LogEvent> ptr;
	LogEvent(std::shared_ptr<Logger>& logger, std::string filename, uint64_t eplase, uint64_t line, std::string threadName, uint64_t threadId, uint64_t fiber, uint64_t date, LogLevel::Level level): m_logger(logger), m_fileName(filename), m_eplase(eplase), m_line(line), m_threadName(threadName), m_threadId(threadId), m_fiber(fiber), m_date(date), m_level(level)
	{
	}
	~LogEvent();


	std::shared_ptr<Logger> getLogger() {return m_logger;}
	std::string getFileName() {return m_fileName;}
	uint64_t getEplase() {return m_eplase;}
	uint64_t getLine() {return m_line;}
	std::string getThreadName() {return m_threadName;}
	uint64_t getThreadId() {return m_threadId;}
	uint64_t getFiber() {return m_fiber;}
	uint64_t getDate() {return m_date;}
	LogLevel::Level getLevel() {return m_level;}
	std::stringstream& getSS() {return m_ss; }

private:
	std::shared_ptr<Logger>& m_logger;
	std::string m_fileName;
	uint64_t m_eplase;
	uint64_t m_line;
	std::string m_threadName;
	uint64_t m_threadId;
	uint64_t m_fiber;
	uint64_t m_date;
	LogLevel::Level m_level;
	std::stringstream m_ss;
};


class LogEventWrap{
public:
	LogEventWrap(LogEvent::ptr event):m_event(event){}
	std::stringstream& getSS(){return m_event->getSS();}
	~LogEventWrap();
private:
	LogEvent::ptr m_event;
};


class LogFormatter{
public:
	typedef std::shared_ptr<LogFormatter> ptr;
	LogFormatter(std::string pattern="%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"):m_pattern(pattern)
	{
		init();
	}

	void log(std::ostream& os, LogEvent::ptr event);
	std::string getPattern()
	{
		return m_pattern;
	}

	class FormatterItem{
	public:

		typedef std::shared_ptr<FormatterItem> ptr;
		virtual ~FormatterItem(){}
		std::string getFormat() {return m_format;}
		virtual void log(std::ostream& os, LogEvent::ptr event, LogLevel::Level level)=0;
	protected:
		std::string m_format;
	};
private:
	void init();
private:
	MutexLock m_mutex;
	std::string m_pattern;
	std::vector<FormatterItem::ptr> m_items;

};


class LogAppender{
public:
	typedef std::shared_ptr<LogAppender> ptr;

	void setFormatter(LogFormatter::ptr formatter){m_formatter = formatter;}

	LogFormatter::ptr getFormatter(){return m_formatter;}
	
	virtual ~LogAppender(){}

	virtual void log(LogEvent::ptr event)=0;
public:
	MutexLock m_mutex;
	LogFormatter::ptr m_formatter;
};


class FileLogAppender:public LogAppender{
public:
	typedef std::shared_ptr<FileLogAppender> ptr;
	FileLogAppender(std::string filename):m_filename(filename)
	{
		std::ofstream m_os(m_filename, std::ios::app);
		m_os<<"jinfeng\n";
	}
	void log(LogEvent::ptr event);
private:
	void reopen();
	std::string m_filename;
	std::ofstream m_os;
};


class StdLogAppender:public LogAppender{
public:
	typedef std::shared_ptr<StdLogAppender> ptr;
	void log(LogEvent::ptr event);
};

class Logger{
public:
	typedef std::shared_ptr<Logger> ptr;

	Logger(std::string name):m_name(name){}

	std::string getName() {return m_name;}

	void addAppender(LogAppender::ptr appender);
	void delAppender(LogAppender::ptr appender);
	void setFormatter(LogFormatter::ptr& formatter);
	LogFormatter::ptr getFormatter();

	void log(LogEvent::ptr event);

private:
	MutexLock m_mutex;
	std::string m_name;
	std::vector<LogAppender::ptr> m_appenders;
	LogFormatter::ptr m_formatter;
};


class LoggerManager{
public:
	typedef std::shared_ptr<LoggerManager> ptr;

	Logger::ptr getRootLogger();

	Logger::ptr getLogger(std::string name);

	static LoggerManager* getInstance();

private:
	LoggerManager();

private:
	static LoggerManager* lmgr;
	//独一份的根 logger
	Logger::ptr m_rootLogger;
	//所管理的 logger 集合
	std::map<std::string, Logger::ptr> m_loggers;
	
};







};
