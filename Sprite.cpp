#ifndef OOSDL_SPRITE_CPP
#define OOSDL_SPRITE_CPP

#include "Sprite.hpp"
#include "Bitmap.cpp"
#include "OOSDL_CONFIG.hpp"

OOSDL::Sprite::Sprite(const Bitmap& bmp, int h, int v, int x, int y)
throw (std::invalid_argument)
: bmp(bmp), width(h), height(v), x(x), y(y), need_update(true), crop(NULL)
{
	if (h <= 0 || v <= 0 || x < 0 || y < 0 || x >= h || y >= v)
	{
		throw std::invalid_argument(OOSDL_SPRITE_TEXT_CONS_INVALIDA);
	}
}

OOSDL::Sprite& OOSDL::Sprite::operator=(const Sprite& rhs)
{
	this->bmp = rhs.getFullBitmap();
	this->width = rhs.getWidth();
	this->height = rhs.getHeight();
	this->x = rhs.getX();
	this->y = rhs.getY();
	this->need_update = true;
}

OOSDL::Sprite::~Sprite()
{
	if (this->crop != NULL)
	{
		delete this->crop;
	}
}

void OOSDL::Sprite::setupCrop()
{
	int sprite_width = this->preferredWidth();
	int sprite_height = this->preferredHeight();
	
	if (this->crop != NULL &&
			(this->crop->getWidth()  != sprite_width  ||
			 this->crop->getHeight() != sprite_height )
		)
	{
		delete this->crop;
	}
	
	if (this->crop == NULL)
	{
		this->crop = new Bitmap(sprite_width, sprite_height);
	}
}

int OOSDL::Sprite::preferredWidth() const
{
	return this->bmp.getWidth() / this->getWidth();
}

int OOSDL::Sprite::preferredHeight() const
{
	return this->bmp.getHeight() / this->getHeight();
}

int OOSDL::Sprite::getX() const
{
	return this->x;
}

int OOSDL::Sprite::getY() const
{
	return this->y;
}

int OOSDL::Sprite::getWidth() const
{
	return this->width;
}

int OOSDL::Sprite::getHeight() const
{
	return this->height;
}

void OOSDL::Sprite::setPosition(int x, int y)
throw (std::invalid_argument)
{
	this->setX(x);
	this->setY(y);
}

void OOSDL::Sprite::setX(int x)
throw (std::invalid_argument)
{
	if (x < 0 || x >= this->getWidth())
	{
		throw std::invalid_argument(OOSDL_SPRITE_TEXT_SETX_INVALIDA);
	}
	
	// Verifica se a coordenada mudou para não atualizar o sprite sem que seja necessário.
	if (this->x != x)
	{
		this->x = x;
		this->need_update = true;
	}
}

void OOSDL::Sprite::setY(int y)
throw (std::invalid_argument)
{
	if (y < 0 || y >= this->getHeight())
	{
		throw std::invalid_argument(OOSDL_SPRITE_TEXT_SETY_INVALIDA);
	}
	
	// Verifica se a coordenada mudou para não atualizar o sprite sem que seja necessário.
	if (this->y != y)
	{
		this->y = y;
		this->need_update = true;
	}
}

const OOSDL::Bitmap& OOSDL::Sprite::getBitmap() const
{
	// Atualiza a imagem do bitmap se o sprite foi alterado.
	if (this->need_update || this->crop == NULL)
	{
		// Burlando validações de constância do compilador.
		Sprite* unconst_this = const_cast<Sprite*>(this);
		
		// Prepara a imagem do sprite.
		unconst_this->setupCrop();
		
		// Calcula o tamanho de um sprite do spriteset.
		int sprite_width = unconst_this->preferredWidth();
		int sprite_height = unconst_this->preferredHeight();
		Rect sprite_rect(sprite_width, sprite_height,
				unconst_this->getX() * sprite_width, unconst_this->getY() * sprite_height);
		
		std::cout << "sprite_rect(width, height, x, y): "
				<< sprite_rect.getWidth() << " "
				<< sprite_rect.getHeight() << " "
				<< sprite_rect.getX() << " "
				<< sprite_rect.getY() << std::endl;
		
		std::cout << "sprite(width, heihgt): "
				<< unconst_this->getWidth() << " "
				<< unconst_this->getHeight() << std::endl;
		
		// Desenha o sprite correto no bitmap.
		unconst_this->crop->drawBitmap(
				unconst_this->getFullBitmap(),
				sprite_rect,
				unconst_this->crop->asRect()
			);
		
		// Marca flag para utilizar o mesmo bitmap já desenhado nas próximas chamadas.
		unconst_this->need_update = false;
	}
	
	return *this->crop;
}

const OOSDL::Bitmap& OOSDL::Sprite::getFullBitmap() const
{
	return this->bmp;
}

#endif
