#ifndef OOSDL_MixerClient_HPP
#define OOSDL_MixerClient_CPP

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "MixerClient.hpp"

//*
int OOSDL::MixerClient::n_initializes[size_of_flags] = {0};

bool OOSDL::MixerClient::usingMix()
{
	bool result = false;
	for (int i = 0; !result && i < size_of_flags; i++)
	{
		result = n_initializes[i];
	}
	return result;
}

OOSDL::MixerClient::MixerClient(int flags)
{
	Mix_Init(flags);
	
	for (int i = 0; i < size_of_flags; i++, flags >> 1)
	{
		if (flags%2)
		{
			n_initializes[i]++;
		}
	}
}

OOSDL::MixerClient::~MixerClient()
{
	int flags = this->flags;
	
	for (int i = 0; i < size_of_flags; i++, flags >> 1)
	{
		if (flags%2)
		{
			n_initializes[i]--;
		}
	}
	
	if (!usingMix())
	{
		MixQuit();
	}
}

/*/
OOSDL::MixerClient::MixerClient()
: SDLClient(0), LibClient<int, int, OOSDL_AUDIO_MIXFLAGS, &Mix_Init, &Mix_Quit>(OOSDL_AUDIO_MIXFLAGS)
{}
//*/

#endif
