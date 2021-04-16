#include "utils.h"
#include <sys/time.h>

#define getTid() syscall(SYS_gettid)


namespace JINFENG{


uint64_t gettid(){

	return getTid();
}

uint64_t getTimeMs()
{
	timeval tm;
	gettimeofday(&tm, nullptr);
	return tm.tv_sec*1000 + tm.tv_usec/1000;
}

};
