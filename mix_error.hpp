#ifndef OOSDL_mix_error_HPP
#define OOSDL_mix_error_HPP

#include "SDL/SDL_mixer.h"
#include "exception.hpp"

namespace OOSDL {
	class mix_error : public exception<&Mix_GetError> {};
};

#endif
