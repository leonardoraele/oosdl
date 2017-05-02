#ifndef OOSDL_Joystick_HPP
#define OOSDL_Joystick_HPP

#include <SDL/SDL.h>
#include "Input.hpp"
#include "sdl_error.hpp"
#include <stdexcept>

namespace OOSDL {
	class Joystick : public Input {
		int index;
		SDL_Joystick* joystick;
		
		bool validateKey(int) const throw (std::invalid_argument);
		Joystick(int index) throw (sdl_error);
		
		// Métodos que não devem ser implementados:
		Joystick(const Joystick&){}
		Joystick& operator=(const Joystick&){}
	public:
		Joystick(Joystick&&);
		~Joystick();
		
		// Métodos de acesso aos joysticks.
		static int getJoystickCount();								// Verifica a quantidade de joysticks existentes.
		static Joystick getJoystick(int index)						// Joga invalid_agument se não houver um joystick associado à este índice.
				throw (std::invalid_argument, sdl_error);
		
		// Método de atualização dos joysticks.
		void update() throw (std::bad_exception);
	};
};
#endif
