#include "Thread.h"

namespace JINFENG{


void* Thread::run (void* args)
{
	Thread* t = (Thread*)args;
	t->m_cb();
}



};
