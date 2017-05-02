#ifndef OOSDL_Lock_CPP
#define OOSDL_Lock_CPP

#include "Lock.hpp"
#include <map>
#include <SDL/SDL.h>

std::map< const void*, SDL_mutex* > OOSDL::Lock::mutex_map;

OOSDL::Lock::Lock(const void* ptr, bool locked)
: key(ptr), locked(false)
{
	// Cria o mutex referente à este lock.
	if (mutex_map.find(this->key) == mutex_map.end())
	{
		mutex_map[this->key] = SDL_CreateMutex();
	}
	
	// Tranca o mutex, se for preciso.
	if (locked)
	{
		this->lock();
	}
}

OOSDL::Lock::~Lock()
{
	this->unlock();
}

bool OOSDL::Lock::isLocked() const
{
	return this->locked;
}

void OOSDL::Lock::lock()
{
	if (!this->isLocked())
	{
		SDL_LockMutex( mutex_map[this->key] );
		this->locked = true;
	}
}

void OOSDL::Lock::unlock()
{
	if (this->isLocked())
	{
		SDL_UnlockMutex( mutex_map[this->key] );
		this->locked = false;
	}
}

#endif
