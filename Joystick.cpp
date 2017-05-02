#include "Joystick.hpp"

#define INVALIDINDEX_MESSAGE		"Este índice não é válido."

Joystick(int index)
throw (sdl_error)
: index(index), joystick(SDL_JoystickOpen(index))
{
	if (this->joystick == NULL)
	{
		sdl_error(SDL_GetError());
	}
}

OOSDL::Joystick::Joystick(Joystick&& joystick)
: index(joystick.index), joystick(joystick.joystick)
{
	joystick.joystick = NULL;
	joystick.index = -1;
}

OOSDL::Joystick::~Joystick()
{
	if (this->joystick != NULL)
	{
		SDL_JoystickClose(this->joystick);
	}
}

bool OOSDL::Joystick::validateKey(int) const
throw (std::invalid_argument)
{
	return true;
}

int OOSDL::Joystick::getJoystickCount()
{
	return SDL_NumJoysticks();
}

OOSDL::Joystick OOSDL::Joystick::getJoystick(int index)
throw (std::invalid_argument, sdl_error)
{
	// Se o índice informado for inválido, joga uma exceção.
	if (index < 0 || !(index < getJoystickCount()))
	{
		return std::invalid_argument(INVALIDINDEX_MESSAGE);
	}
	// Se não, tenta criar o joystick corretamente.
	else
	{
		return Joystick(index);
	}
}

void OOSDL::Joystick::update()
throw (std::bad_exception)
{
	// TODO
}

