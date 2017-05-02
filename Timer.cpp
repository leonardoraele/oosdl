#ifndef OOSDL_Timer_CPP
#define OOSDL_Timer_CPP

#include <string>
#include <SDL/SDL.h>
#include "Timer.hpp"
#include "Runnable.hpp"

OOSDL::Timer::Timer(Runnable* runnable, Uint32 interval, std::string name)
: runnable(runnable), interval(interval), name(name), running(false), id(0)
{}

OOSDL::Timer::Timer(const Timer& rhs)
: runnable(rhs.runnable), interval(rhs.interval), name(rhs.name), running(rhs.running), id(rhs.id)
{}

OOSDL::Timer& OOSDL::Timer::operator=(const Timer& rhs)
{
	this->runnable = rhs.runnable;
	this->interval = rhs.interval;
	this->name = rhs.name;
	if (rhs.isScheduled())
	{
		this->running = rhs.running;
		this->id = rhs.id;
	}
	else
	{
		this->running = false;
		this->id = 0;
	}
}

OOSDL::Timer::~Timer()
{
	if (this->isScheduled())
	{
		this->cancel();
	}
}

Uint32 OOSDL::Timer::timer_func(Uint32 interval, void* param)
{
	void** params = (void**) param;
	void* void_runnable = params[0];
	param = params[1];
	
	Runnable* runnable = static_cast<Runnable*>(void_runnable);
	//TODO// delete params;
	runnable->run(param);
	
	// TODO Chama bugada?
	SDL_AddTimer(interval, &timer_func, params);
	
	return 0;
}

void OOSDL::Timer::start(void* param, Uint32 delay)
{
	void** params = new void*[2];
	params[0] = this->runnable;
	params[1] = param;
	param = (void*) params;
	
	this->id = SDL_AddTimer(this->getInterval(), &timer_func, param);
	
	this->running = true;
}

void OOSDL::Timer::cancel()
{
	this->running = !SDL_RemoveTimer(this->id);
}

bool OOSDL::Timer::isScheduled() const
{
	return this->running;
}

Uint32 OOSDL::Timer::getInterval() const
{
	return this->interval;
}

const std::string& OOSDL::Timer::getName() const
{
	return this->name;
}

SDL_TimerID OOSDL::Timer::get_SDL_Timer()
{
	return this->id;
}

Uint32 OOSDL::Timer::getCurrentTime()
{
	return SDL_GetTicks();
}

#endif
