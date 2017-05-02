#ifndef OOSDL_NetClient_CPP
#define OOSDL_NetClient_CPP

#include <SDL/SDL_net.h>
#include "NetClient.hpp"

int OOSDL::SDLNet_Init(const void*)
{
	return ::SDLNet_Init();
}

#endif
