#ifndef OOSDL_net_error_CPP
#define OOSDL_net_error_CPP

#include <SDL/SDL_net.h>
#include "net_error.hpp"

OOSDL::net_error::net_error()
: sdl_error(SDLNet_GetError())
{}

OOSDL::net_error::net_error(const char* error_msg)
: sdl_error(error_msg)
{}

#endif
