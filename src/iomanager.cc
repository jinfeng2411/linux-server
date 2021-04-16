#include "iomanager.h"
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "log.h"

namespace JINFENG{

static JINFENG::Logger::ptr system = JINFENG_LOG_NAME("system");

IOManager::FdContext::EventContext& IOManager::FdContext::getContext(Event event)
{
	if(event==IOManager::READ)
		return read;
	if(event==IOManager::WRITE)
		return write;
	assert(0);
}

void IOManager::FdContext::resetContext(EventContext& ctx)
{
	ctx.schedule = nullptr;
	ctx.cb = nullptr;
}

void IOManager::FdContext::triggerEvent(Event event)
{
	
	events = (Event)(events & ~event);
	EventContext& ctx = getContext(event);

	if(ctx.cb)
	{
		ctx.schedule->schedule(ctx.cb, -1);
	}
	else
	{
		Fiber::ptr p(ctx.fiber);
		ctx.schedule->schedule(p, -1);
	}
	ctx.schedule = nullptr;
	return;
	
}

//初始化好 epfd;
//并将 m_tickleFd 加入 epfd 的监听中
IOManager::IOManager(std::string name, size_t threads, bool use_caller):Schedule(name, threads, use_caller)
{
	JINFENG_LOG_DEBUG(system)<<"begin IOManager";
	m_epfd = epoll_create(5500);
	assert(m_epfd>0);

	int rt = pipe(m_tickleFds);
	assert(rt==0);

	epoll_event event;
	memset(&event, 0, sizeof(epoll_event));
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = m_tickleFds[0];
	
	rt = fcntl(m_tickleFds[0], F_SETFL, O_NONBLOCK);
	assert(!rt);

	rt = epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_tickleFds[0], &event);
	assert(!rt);

	JINFENG_LOG_DEBUG(system)<<"end IOManager.";
	//Schedule 的start()，创建线程池，每个线程进入 Schedule 的run() 函数
	start();
}

IOManager::~IOManager()
{
	close(m_epfd);
	close(m_tickleFds[0]);
	close(m_tickleFds[1]);

	for(size_t i=0; i<m_fdContexts.size(); i++)
	{
		if(m_fdContexts[i])
		{
			delete m_fdContexts[i];
		}
	}
}

void IOManager::contextResize(size_t size)
{
	m_fdContexts.resize(size);

	for(size_t i=0;i<m_fdContexts.size();++i){
		if(!m_fdContexts[i])
		{
			m_fdContexts[i] = new FdContext;
			m_fdContexts[i]->fd = i;
		}
	}
}

int IOManager::addEvent(int fd, Event event, std::function<void()> cb)
{
	FdContext* fd_ctx = nullptr;
	if(m_fdContexts.size() > fd)
	{
		fd_ctx = m_fdContexts[fd];
	}
	else
	{
		contextResize(fd*1.5);
		fd_ctx = m_fdContexts[fd];
	}
	int op = fd_ctx->events ? EPOLL_CTL_MOD : EPOLL_CTL_ADD;
	epoll_event epevent;
	epevent.events = EPOLLET | fd_ctx->events | event;
	epevent.data.ptr = fd_ctx;

	int rt = epoll_ctl(m_epfd, op, fd, &epevent);
	assert(!rt);

	fd_ctx->events = (Event)(fd_ctx->events | event);
	FdContext::EventContext& event_ctx = fd_ctx->getContext(event);
	event_ctx.schedule = Schedule::getThis();
	if(cb)
		event_ctx.cb = cb;
	else{
		event_ctx.fiber = Fiber::getThis();
	}
	return 0;
}

void IOManager::tickle()
{
	JINFENG_LOG_DEBUG(system)<<"in IOManager::tickle";
	int rt = write(m_tickleFds[1], "T", 1);
	assert(rt==1);
}

bool IOManager::stopping()
{

}

void IOManager::idle()
{
	JINFENG_LOG_DEBUG(system)<<"in idle.";
	epoll_event* events = new epoll_event[256]();
	std::shared_ptr<epoll_event> shared_events(events);

	int rt;
	while(1){
		while(1){
			uint64_t next_timeout;
			{
			JINFENG::MutexLockGuard lock(m_mutex);
			next_timeout = (int)getNextTimer();
			JINFENG_LOG_DEBUG(system)<<" : next_timeout: "<<next_timeout;
			}
			next_timeout = next_timeout>3000 ? 3000 : next_timeout;

			rt = epoll_wait(m_epfd, events, 256, (int)next_timeout);
			if(rt<0 && errno==EINTR)  //发生错误，继续阻塞在 epoll_wait
			{
			}
			else  //否认，真有事件发生，跳出来处理事件
				break;
		}
		JINFENG_LOG_DEBUG(system)<<"break epoll_wait,";
		//rt 为触发事件数量
		for(int i=0;i<rt;++i)
		{
			epoll_event& event = events[i];
			if(event.data.fd == m_tickleFds[0])  //如果是 tickle() 函数触发的
			{
				uint8_t dummy[256];  //把管道中的消息读完
				while(read(m_tickleFds[0], dummy, sizeof(dummy))>0);
				continue;
			}
			//获取 event 事件对应的结构体
			FdContext* fd_ctx = (FdContext*)event.data.ptr;
			if(event.events & (EPOLLERR | EPOLLHUP))
			{
				event.events |= EPOLLIN | EPOLLOUT;
			}

			int real_events = NONE;
			if(event.events & EPOLLIN){
				real_events |= READ;
			}
			if(event.events & EPOLLOUT){
				real_events |= WRITE;
			}
			//触发的事件不是监听的事件
			if((fd_ctx->events & real_events)==NONE)
				continue;
			//除去触发事件后剩下的事件， 继续监听
			int left_events = (fd_ctx->events & ~real_events);
			int op = left_events ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;
			event.events = EPOLLET | left_events;

			int ret = epoll_ctl(m_epfd, op, fd_ctx->fd, &event);
			assert(!ret);

			//若触发事件中有读事件，则处理
			if(fd_ctx->events & READ){
				fd_ctx->triggerEvent(READ);
			}

			//若触发事件中有写事件，则处理
			if(fd_ctx->events & WRITE){
				fd_ctx->triggerEvent(WRITE);
			}
		}
		//处理定时器
		handleTimer(JINFENG::getTimeMs());
		JINFENG_LOG_DEBUG(system)<<"will jump out IOManager::idle";
		JINFENG::Fiber::YieldToHold();
	}
}






};
