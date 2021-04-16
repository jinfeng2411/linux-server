#include "log.h"
#include "utils.h"
#include <iostream>

int main(){
	JINFENG::Logger::ptr logger(new JINFENG::Logger("jinfeng"));

	JINFENG::LogFormatter::ptr formatter(new JINFENG::LogFormatter());

	logger->setFormatter(formatter);

	JINFENG::LogAppender::ptr stdAppender(new JINFENG::StdLogAppender());
	JINFENG::LogAppender::ptr fileAppender(new JINFENG::FileLogAppender("./log.txt"));


	logger->addAppender(stdAppender);
	logger->addAppender(fileAppender);



	JINFENG_LOG_DEBUG(logger)<<"debug";
	JINFENG_LOG_INFO(logger)<<"info";
	JINFENG_LOG_WARN(logger)<<"warn";
	JINFENG_LOG_ERROR(logger)<<"error";
	JINFENG_LOG_FATAL(logger)<<"fatal";
	return 0;
}
