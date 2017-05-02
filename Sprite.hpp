#ifndef OOSDL_SPRITE_HPP
#define OOSDL_SPRITE_HPP

#include "Bitmap.hpp"
#include <exception>

namespace OOSDL {
	class Sprite {
		Bitmap bmp;
		Bitmap* crop;
		int width;
		int height;
		int x;
		int y;
		bool need_update;
		
		void setupCrop();
		int preferredWidth() const;
		int preferredHeight() const;
	public:
		Sprite(const Bitmap& bmp, int h = 1, int v = 1, int x = 0, int y = 0)	// Cria um spriteset a partir de uma imagem que contém h sprites na horizontal e v sprites na vertical.
				throw (std::invalid_argument);
		Sprite& operator=(const Sprite&);
		virtual ~Sprite();
		
		// Posição do sprite:
		virtual int getX() const;
		virtual int getY() const;
		virtual int getWidth() const;
		virtual int getHeight() const;
		
		virtual void setPosition(int x, int y) throw (std::invalid_argument);
		virtual void setX(int x) throw (std::invalid_argument);
		virtual void setY(int y) throw (std::invalid_argument);
		
		// Leitura da imagem do sprite:
		virtual const Bitmap& getBitmap() const;
		virtual const Bitmap& getFullBitmap() const;
	};
};

#endif
