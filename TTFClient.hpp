#ifndef OOSDL_TTFClient_HPP
#define OOSDL_TTFClient_HPP

#include <SDL/SDL_ttf.h>
#include "LibClient.cpp"

namespace OOSDL {
	int TTF_Init(const void*);
	
	typedef LibClient<int, const void*, 0, &TTF_Init, &TTF_Quit> TTFClient;
};

#endif
