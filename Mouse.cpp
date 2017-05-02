#ifndef OOSDL_Mouse_CPP
#define OOSDL_Mouse_CPP

#include <iostream>
#include "Mouse.hpp"

#define INVALIDKEY_MESSAGE		"Botão inexistente."

OOSDL::Mouse* OOSDL::Mouse::instance = NULL;

OOSDL::Mouse::Mouse()
: Input()
{}

OOSDL::Mouse* OOSDL::Mouse::getInstance()
{
	if (instance == NULL)
	{
		instance = new Mouse();
	}
	return instance;
}

int OOSDL::Mouse::getMouseX() const
{
	return this->mouse_x;
}

int OOSDL::Mouse::getMouseY() const
{
	return this->mouse_y;
}

void OOSDL::Mouse::update()
throw (std::bad_exception)
{
	Uint8 buttonmask;
	buttonmask = SDL_GetMouseState(&this->mouse_x, &this->mouse_y);
	
	// TODO Testar:
	for (int i = 0; i < sizeof(Uint8); i++)
	{
		Uint8 keymask = (1 << i);
		if (keymask & buttonmask)
		{
			this->push(keymask);
		}
		else
		{
			this->release(keymask);
		}
	}
}

#endif
