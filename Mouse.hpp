#ifndef OOSDL_Mouse_HPP
#define OOSDL_Mouse_HPP

#include <SDL/SDL.h>
#include <exception>
#include "Window.hpp"
#include "Input.hpp"

namespace OOSDL {
	class Mouse : public Input {
		int mouse_x;
		int mouse_y;
		
		static Mouse* instance;
		Mouse();
	public:
		static Mouse* getInstance();
		
		enum MouseKey { MouseKey = 0
			, Left		 = 0b00000001
			, Center	 = 0b00000010
			, Right		 = 0b00000100
		, MouseKey_End};
		
		// Verifica a posição do mouse.
		virtual int getMouseX() const;
		virtual int getMouseY() const;
		
		virtual void update() throw (std::bad_exception);
	};
};
#endif
