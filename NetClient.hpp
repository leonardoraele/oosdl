#ifndef OOSDL_NetClient_HPP
#define OOSDL_NetClient_HPP

#include <SDL/SDL_net.h>
#include "LibClient.hpp"
#include "SDLClient.hpp"

namespace OOSDL {
	int SDLNet_Init(const void*);
	
	class NetClient : public SDLClient, LibClient<int, const void*, 0, &SDLNet_Init, &SDLNet_Quit> {
	public:
		NetClient();
	};
};

#endif
