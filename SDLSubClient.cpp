#ifndef OOSDL_SDLSubClient_CPP
#define OOSDL_SDLSubClient_CPP

#include "SDLSubClient.hpp"
#include "SDLClient.cpp"
#include "LibClient.cpp"

template <Uint32 SubSystem>
void OOSDL::QuitSubSystem(void)
{
	SDL_QuitSubSystem(SubSystem);
}

template <Uint32 SubSystem>
OOSDL::SDLSubClient<SubSystem>::SDLSubClient()
: SDLClient(0), LibClient<int, Uint32, 0, &SDL_InitSubSystem, &QuitSubSystem<SubSystem> >(SubSystem)
{}

#endif
