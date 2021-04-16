#include "fiber.h"
#include <iostream>
namespace JINFENG{

static thread_local int fiber_count = 1;
static thread_local Fiber::ptr main_fiber;
static thread_local Fiber* running_fiber;

Fiber::Fiber(std::function<void()> cb):m_cb(cb)
{
	int ret = getcontext(&m_ctx);
	m_ctx.uc_stack.ss_sp = m_stack;
	m_ctx.uc_stack.ss_size = sizeof(m_stack);
	m_ctx.uc_stack.ss_flags = 0;
	makecontext(&m_ctx, &mainFunc, 0);
	m_id = fiber_count;
	++fiber_count;
}

Fiber::Fiber()
{
	m_state = EXEC;
	m_id = 0;
	m_name = "main";
	getcontext(&m_ctx);
}


void Fiber::setThis(Fiber* f)
{
	running_fiber = f;
}

void Fiber::swapIn()
{
	setThis(this);
	m_state = EXEC;
	swapcontext(&main_fiber->getCtx(), &m_ctx);
}

Fiber* Fiber::getThis()
{
	if(running_fiber)
		return running_fiber;
	Fiber::ptr f(new Fiber());
	main_fiber = f;
}

void Fiber::swapOut()
{
	setThis(main_fiber.get());
	swapcontext(&m_ctx, &main_fiber->getCtx());
}

void Fiber::YieldToHold()
{
	Fiber* f = getThis();
	f->swapOut();	
}

uint64_t Fiber::getFiberId()
{
	return m_id;
}


};
