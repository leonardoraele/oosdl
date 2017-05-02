#ifndef OOSDL_Color_HPP
#define OOSDL_Color_HPP

#include <SDL/SDL.h>

namespace OOSDL {
	class Color {
		Uint32 color;
		Uint8* red;
		Uint8* green;
		Uint8* blue;
		Uint8* alpha;
		
		void adjustPointers();								// Faz os ponteiros de cada cor individual apontar para o objeto color que guarda as cores.
	public:
		// Construtores.
		Color(Uint32 color);
		Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);	// Cria um objeto que represente uma cor no formato RGB de transparência A(por padrão, completamente opaco).
		Color(const Color&);
		virtual ~Color(){}
		
		// Operatores.
		virtual Color& operator=(const Color&);
		
		// Filtro de cor
		virtual Uint8 getRed() const;						// Recupera a cor vermelha.
		virtual Uint8 getGreen() const;						// Recupera a cor verde.
		virtual Uint8 getBlue() const;						// Recupera a cor verde.
		virtual Uint8 getAlpha() const;						// Recupera a transparência.
		
		// Altera a cor.
		virtual Color& setRed(Uint8);						// Altera o valor vermelho da cor.
		virtual Color& setGreen(Uint8);						// Altera o valor verde da cor.
		virtual Color& setBlue(Uint8);						// Altera o valor azul da cor.
		virtual Color& setAlpha(Uint8);						// Altera a transparência da cor.
		
		// Leitura de transparência.
		/*(nenhum desses métodos é mais eficiente que realizar as condições propostas com o método getAlpha(); eles servem somente para facilitar a leitura do código)*/
		virtual bool isRGB() const;							// Alpha == 255? !isRGBA()?
		virtual bool isRGBA() const;						// Alpha != 255? !isRGB()?
		virtual bool isInvisible() const;					// Alpha == 0? !isVisible()?
		virtual bool isVisible() const;						// Alpha != 0? !isInvisible()?
		
		// Recupera uma variável que representa a cor no formato SDL.
		virtual Uint32 to_SDL_Uint32() const;
		virtual SDL_Color to_SDL_Color() const;
		
		// Cores padrões para facilitar o uso:
		static  Color Black;
		static  Color White;
		static  Color DarkGray;
		static  Color LightGray;
		static  Color Red;
		static  Color Green;
		static  Color Blue;
		static  Color Yellow;
		static  Color DarkPink;
		static  Color LightPink;
		static  Color Brown;
		static  Color Orange;
		static  Color Purple;
		static  Color Cyan;
	};
};
#endif
