#ifndef OOSDL_Rect_HPP
#define OOSDL_Rect_HPP

#include <SDL/SDL.h>

namespace OOSDL {
	class Rect {
		SDL_Rect rect;
	public:
		Rect(Uint16 width = 0, Uint16 height = 0, Sint16 x = 0, Sint16 y = 0);
		virtual ~Rect(){};
		virtual Rect& operator=(const Rect&);
		
		virtual Sint16 getX() const;
		virtual Sint16 getY() const;
		virtual Uint16 getWidth() const;
		virtual Uint16 getHeight() const;
		
		virtual Rect& setX(Sint16);
		virtual Rect& setY(Sint16);
		virtual Rect& setWidth(Uint16);
		virtual Rect& setHeight(Uint16);
		
		// Métodos de verificação.
		virtual bool isSquare() const;									// Verifica se este retângulo é um quadrado perfeito. (width == height)
		virtual bool isVisible() const;									// Verifica se a largura e comprimento deste retângulo são visíveis. (width > 0 && height > 0)
		virtual bool isVisible(const Rect& rect) const;					// Verifica se alguma parte deste retângulo é visível dentro do retângulo rect.
		virtual bool isInside(const Rect& rect) const;					// Verifica se este retângulo está completamente contido dentro do retângulo rect.
		
		// Recupera o objeto SDL_Rect referente à este retângulo.
		virtual SDL_Rect to_SDL_Rect() const;
	};
};
#endif
