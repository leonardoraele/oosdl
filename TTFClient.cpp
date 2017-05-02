#ifndef OOSDL_TTFClient_CPP
#define OOSDL_TTFClient_CPP

#include <SDL/SDL_ttf.h>
#include "TTFClient.hpp"

int OOSDL::TTF_Init(const void*)
{
	return ::TTF_Init();
}

#endif
