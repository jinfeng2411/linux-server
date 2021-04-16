#pragma once

#include<stdint.h>
#include<sys/syscall.h>
#include<unistd.h>

namespace JINFENG{
	uint64_t gettid();


	uint64_t getTimeMs();

};
