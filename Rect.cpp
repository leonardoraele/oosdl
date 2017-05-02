#ifndef OOSDL_Rect_CPP
#define OOSDL_Rect_CPP
#include "Rect.hpp"

OOSDL::Rect::Rect(Uint16 width, Uint16 height, Sint16 x, Sint16 y)
{
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
}

OOSDL::Rect& OOSDL::Rect::operator=(const Rect& rhs)
{
	this->rect.x = rhs.rect.x;
	this->rect.y = rhs.rect.y;
	this->rect.w = rhs.rect.w;
	this->rect.h = rhs.rect.h;
}

Sint16 OOSDL::Rect::getX() const {
	return this->rect.x;
}

Sint16 OOSDL::Rect::getY() const {
	return this->rect.y;
}

Uint16 OOSDL::Rect::getWidth() const {
	return this->rect.w;
}

Uint16 OOSDL::Rect::getHeight() const {
	return this->rect.h;
}

OOSDL::Rect& OOSDL::Rect::setX(Sint16 x) {
	this->rect.x = x;
	return (*this);
}

OOSDL::Rect& OOSDL::Rect::setY(Sint16 y) {
	this->rect.y = y;
	return (*this);
}

OOSDL::Rect& OOSDL::Rect::setWidth(Uint16 width)
{
	this->rect.w = width;
	return (*this);
}

OOSDL::Rect& OOSDL::Rect::setHeight(Uint16 height)
{
	this->rect.h = height;
	return (*this);
}

bool OOSDL::Rect::isSquare() const
{
	return (this->getWidth() == this->getHeight());
}

bool OOSDL::Rect::isVisible() const
{
	return (this->getWidth() > 0 && this->getHeight() > 0);
}

bool OOSDL::Rect::isVisible(const Rect& rect) const
{
	return rect.isVisible() &&
		// Verifica se o retângulo rect contém parte deste retângulo.
		(
			(this->getX() + this->getWidth() >= rect.getX() && this->getX() <= rect.getX() + rect.getWidth())	// Verifica se contém horizontalmente.
			&&
			(this->getY() + this->getHeight() >= rect.getY() && this->getY() <= rect.getY() + rect.getHeight())	// Verifica se contém verticalmente.
		);
}

bool OOSDL::Rect::isInside(const Rect& rect) const
{
	return rect.isVisible() &&
		// Verifica se o retângulo rect contém completamente este retângulo.
		(
			(this->getX() >= rect.getX() && this->getX() + this->getWidth() <= rect.getX() + rect.getWidth())	// Verifica se contém horizontalmente.
			&&
			(this->getY() >= rect.getY() && this->getY() + this->getHeight() <= rect.getY() + rect.getHeight())	// Verifica se contém verticalmente.
		);
}

SDL_Rect OOSDL::Rect::to_SDL_Rect() const {
	// As abordagens abaixo são equivalentes, porém
	// a primeira é mais legível e a segunda é mais otimizada.
	// OBS: A segunda só é válida em C++11.
	
	//*/ <switch>
	
	SDL_Rect result;
	result.x = this->getX();
	result.y = this->getY();
	result.w = this->getWidth();
	result.h = this->getHeight();
	return result;
	
	/*/
	
	return {
			this->getX(),
			this->getY(),
			this->getWidth(),
			this->getHeight()
		};
	
	//*/
	
}

#endif
