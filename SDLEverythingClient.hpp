#ifndef OOSDL_SDLEverythingClient_HPP
#define OOSDL_SDLEverythingClient_HPP

#include "SDLSubClient.hpp"

#include "SDLTimerClient.hpp"
#include "SDLAudioClient.hpp"
#include "SDLVideoClient.hpp"
#include "SDLCDROMClient.hpp"
#include "SDLJoystickClient.hpp"

namespace OOSDL {
	class SDLEverythingClient
	: public SDLTimerClient, SDLAudioClient, SDLVideoClient, SDLCDROMClient, SDLJoystickClient
	{};
};

#endif
