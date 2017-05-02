#ifndef OOSDL_Input_CPP
#define OOSDL_Input_CPP
#include "Input.hpp"
#include <string>
#include <algorithm>
#include <stdexcept>

#define DEFAULT_SIZE			10

std::vector<SDL_Event> OOSDL::Input::searchEvents(Uint32 mask)
{
	std::vector<SDL_Event> result(DEFAULT_SIZE);
	SDL_Event events[DEFAULT_SIZE];
	int numevents;
	SDL_PumpEvents();
	while((numevents = SDL_PeepEvents(events, DEFAULT_SIZE, SDL_GETEVENT, mask)) && numevents > 0)
	{
		for (int i = 0; i < numevents; i++)
		{
			result.push_back(events[i]);
		}
	}
}

bool OOSDL::Input::isDown(int key) const
{
	return (this->get(key) > RELEASED);
}

bool OOSDL::Input::trigger(int key) const
{
	return (this->get(key) == TRIGGERED);
}

bool OOSDL::Input::press(int key) const
{
	return (this->get(key) == PUSHED);
}

bool OOSDL::Input::trigger() const
{
	bool keyDown = false;
	for (auto it = this->keys.begin(); !keyDown && it != this->keys.end(); it++)
	{
		keyDown = this->isDown(it->first);
	}
	return keyDown;
}

void OOSDL::Input::reset()
{
	for (auto it = this->keys.begin(); it != this->keys.end(); it++)
	{
		this->release(it->first);
	}
}

int OOSDL::Input::get(int key) const
{
	// Procura pelo elemento no mapa.
	auto it = this->keys.find(key);
	int result;
	
	// Se encontrar, retorna seu valor.
	if (it != this->keys.end())
	{
		result =  it->second;
	}
	
	// Se não, retorna um valor padrão.
	else
	{
		result = KeyState + 1;		// Retorna o primeiro estado possível. (RELEASED)
	}
	
	return result;
}

void OOSDL::Input::set(int key, int value)
{
	this->keys[key] = value;
}

void OOSDL::Input::push(int key)
{
	int value = std::min(this->get(key) + 1, KeyState_End - 1);
	this->set(key, value);
}

void OOSDL::Input::release(int key)
{
	this->set(key, KeyState + 1);
}
#endif
