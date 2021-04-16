#pragma once
#include<ucontext.h>
#include<memory>
#include<string>
#include<functional>
#include<atomic>
#include<stdint.h>

namespace JINFENG{

class Fiber: public std::enable_shared_from_this<Fiber>{
public:

	enum states{
		READY = 0,
		EXEC = 1,
		HOLD = 2,
		TERM = 3,
		EXCP = 4
	};

	typedef std::shared_ptr<Fiber> ptr;

	Fiber(std::function<void()> cb);


	void setThis(Fiber*);

	static Fiber* getThis();

	void swapIn();

	void swapOut();

	static void YieldToHold();

	uint64_t getFiberId();

	ucontext_t& getCtx() {return m_ctx;}

	static void mainFunc()
	{
		Fiber* f = getThis();
		f->m_cb();
		f->swapOut();
	}

private:
	Fiber();

private:
	ucontext_t m_ctx;
	char m_stack[128*1024];
	states m_state;
	uint64_t m_id;
	std::string m_name;
	std::function<void()> m_cb;
};



};
