#ifndef OOSDL_Keyboard_HPP
#define OOSDL_Keyboard_HPP

#include <SDL/SDL.h>
#include <stdexcept>
#include <exception>
#include "Input.cpp"

namespace OOSDL {
	class Keyboard : public Input {
		static Keyboard instance;
		Keyboard();
	public:
		static Keyboard& getInstance();
		
		virtual void update() throw (std::bad_exception);
	};
};
#endif
