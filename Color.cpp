#ifndef OOSDL_Color_CPP
#define OOSDL_Color_CPP
#include "Color.hpp"

OOSDL::Color OOSDL::Color::Black		(0, 0, 0);
OOSDL::Color OOSDL::Color::White		(255, 255, 255);
OOSDL::Color OOSDL::Color::DarkGray		(63, 63, 63);
OOSDL::Color OOSDL::Color::LightGray	(127, 127, 127);
OOSDL::Color OOSDL::Color::Red			(255, 0, 0);
OOSDL::Color OOSDL::Color::Green		(0, 255, 0);
OOSDL::Color OOSDL::Color::Blue			(0, 0, 255);
OOSDL::Color OOSDL::Color::Yellow		(255, 255, 0);
OOSDL::Color OOSDL::Color::DarkPink		(255, 0, 127);
OOSDL::Color OOSDL::Color::LightPink	(255, 127, 191);
OOSDL::Color OOSDL::Color::Brown		(127, 31, 0);
OOSDL::Color OOSDL::Color::Orange		(255, 127, 0);
OOSDL::Color OOSDL::Color::Purple		(127, 0, 191);
OOSDL::Color OOSDL::Color::Cyan			(63, 191, 191);

OOSDL::Color::Color(Uint32 color)
: color(color)
	, red(NULL), green(NULL), blue(NULL), alpha(NULL)
{
	this->adjustPointers();
}

OOSDL::Color::Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
: color(0)
	, red(NULL), green(NULL), blue(NULL), alpha(NULL)
{
	this->adjustPointers();
	
	*this->red		 = red;
	*this->green	 = green;
	*this->blue		 = blue;
	*this->alpha	 = alpha;
}

OOSDL::Color::Color(const Color& other)
: color(other.color)
	, red(NULL), green(NULL), blue(NULL), alpha(NULL)
{
	this->adjustPointers();
}

void OOSDL::Color::adjustPointers()
{
	Uint8* p = reinterpret_cast<Uint8*>(&this->color);
	
	this->red		= p + 2;
	this->green		= p + 1;
	this->blue		= p + 0;
	this->alpha		= p + 3;
}

OOSDL::Color& OOSDL::Color::operator=(const Color& rhs)
{
	this->color = rhs.color;
	return (*this);
}

Uint8 OOSDL::Color::getRed() const {
	return *this->red;
}

Uint8 OOSDL::Color::getGreen() const {
	return *this->green;
}

Uint8 OOSDL::Color::getBlue() const {
	return *this->blue;
}

Uint8 OOSDL::Color::getAlpha() const {
	return *this->alpha;
}

OOSDL::Color& OOSDL::Color::setRed(Uint8 red)
{
	*this->red = red;
	return (*this);
}

OOSDL::Color& OOSDL::Color::setGreen(Uint8 green)
{
	*this->green = green;
	return (*this);
}

OOSDL::Color& OOSDL::Color::setBlue(Uint8 blue)
{
	*this->blue = blue;
	return (*this);
}

OOSDL::Color& OOSDL::Color::setAlpha(Uint8 alpha)
{
	*this->alpha = alpha;
	return (*this);
}

bool OOSDL::Color::isRGB() const {
	return (this->getAlpha() == 255);
}

bool OOSDL::Color::isRGBA() const {
	return !this->isRGB();
}

bool OOSDL::Color::isInvisible() const {
	return (this->getAlpha() == 0);
}

bool OOSDL::Color::isVisible() const {
	return !this->isInvisible();
}

Uint32 OOSDL::Color::to_SDL_Uint32() const
{
	return color;
}

SDL_Color OOSDL::Color::to_SDL_Color() const
{
	SDL_Color result;
	result.r = this->getRed();
	result.g = this->getGreen();
	result.b = this->getBlue();
	return result;
}

#endif
