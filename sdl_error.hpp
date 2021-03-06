#ifndef OOSDL_sdl_error_HPP
#define OOSDL_sdl_error_HPP

#include "SDL/SDL.h"
#include "exception.cpp"

namespace OOSDL {
	class sdl_error : public exception<&SDL_GetError> {};
};

#endif
