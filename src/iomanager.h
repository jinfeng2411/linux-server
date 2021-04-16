#pragma once
#include "Schedule.h"
#include "Timer.h"
#include <sys/epoll.h>

namespace JINFENG{

class IOManager : public Schedule, public TimerManager{
public:
	typedef std::shared_ptr<IOManager> ptr;

	enum Event{
		NONE = 0x0,
		READ = 0x1,
		WRITE = 0x4,
	};

private:
	struct FdContext{
		struct EventContext{
			Schedule* schedule = nullptr;
			Fiber* fiber;
			std::function<void()> cb;
		};

		EventContext& getContext(Event event);

		void resetContext(EventContext& ctx);

		void triggerEvent(Event event);

		EventContext read;
		EventContext write;

		int fd = 0;

		Event events = NONE;
	};
public:
	IOManager(std::string name="", size_t threads=1, bool use_caller=false);

	~IOManager();

	int addEvent(int fd, Event event, std::function<void()> cb=nullptr);

	bool delEvent(int fd, Event event);

	static IOManager* GetThis();

protected:
	void tickle() override;

	bool stopping();

	void idle() override;

	void contextResize(size_t size);

private:
	int m_epfd = 0;
	int m_tickleFds[2];
	
	std::vector<FdContext*> m_fdContexts;
};


};
