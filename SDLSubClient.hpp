#ifndef OOSDL_SDLSubClient_HPP
#define OOSDL_SDLSubClient_HPP

#include <SDL/SDL.h>
#include "SDLClient.cpp"

namespace OOSDL {
	template <Uint32 SubSystem>
	void QuitSubSystem(void);
	
	template <Uint32 SubSystem>
	class SDLSubClient : public SDLClient, public LibClient<int, Uint32, 0, &SDL_InitSubSystem, &QuitSubSystem<SubSystem> > {
	public:
		SDLSubClient();
	};
};

#endif
