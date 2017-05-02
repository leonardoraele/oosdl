#ifndef OOSDL_SDLClient_HPP
#define OOSDL_SDLClient_HPP

#include <SDL/SDL.h>
#include "LibClient.cpp"

namespace OOSDL {
	typedef LibClient<int, Uint32, 0, &SDL_Init, &SDL_Quit> SDLClient;
};

#endif
