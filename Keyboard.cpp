#ifndef OOSDL_Keyboard_CPP
#define OOSDL_Keyboard_CPP
#include "Keyboard.hpp"
#include "Input.cpp"
#include <vector>
#include <iostream>

#define NUM_EVENTS				10
#define INVALIDKEY_MESSAGE		"Tecla inexistente."

OOSDL::Keyboard OOSDL::Keyboard::instance = OOSDL::Keyboard();

OOSDL::Keyboard::Keyboard()
: Input()
{}

OOSDL::Keyboard& OOSDL::Keyboard::getInstance()
{
	return instance;
}

void OOSDL::Keyboard::update()
throw (std::bad_exception)
{
	SDL_PumpEvents();
	SDL_Event events[NUM_EVENTS];
	int numevents;
	
	// Procura por eventos de teclado.
	while (numevents = SDL_PeepEvents(events, NUM_EVENTS, SDL_GETEVENT, SDL_KEYUPMASK|SDL_KEYDOWNMASK))
	{
		// Verifica se houve erro ao recuperar eventos.
		if (numevents != -1)
		{
			// Percorre cada evento recuperado.
			for (int i = 0; i < numevents; i++)
			{
				// Verifica qual a tecla que está sendo utilizada.
				int key = events[i].key.keysym.sym;
				
				// Verifica o tipo de evento que utiliza a tecla.
				if (events[i].type == SDL_KEYUP)
				{
					this->release(key);
				}
				else // KEYDOWN
				{
					this->push(key);
				}
			}
		}
		else
		{
			// TODO Tratar corretamente.
		}
	}
	
}
#endif
