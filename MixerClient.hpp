#ifndef OOSDL_MixerClient_HPP
#define OOSDL_MixerClient_HPP

#include <SDL/SDL_mixer.h>
#include "OOSDL_CONFIG.hpp"
#include "SDLClient.hpp"
#include "LibClient.hpp"

namespace OOSDL {
	//*
	class MixerClient {
		int flags;
		
		static const short size_of_flags = 32;
		static int n_initializes[size_of_flags];
		
		static bool usingMix();
	public:
		MixerClient(int flags);
		~MixerClient();
	};
	
	/*/
	class MixerClient : public SDLClient, LibClient<int, int, OOSDL_AUDIO_MIXFLAGS, &Mix_Init, &Mix_Quit> {
	public:
		MixerClient();
	};
	//*/
};

#endif
