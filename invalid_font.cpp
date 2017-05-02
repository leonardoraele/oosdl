#ifndef OOSDL_invalid_font_CPP
#define OOSDL_invalid_font_CPP
#include "invalid_font.hpp"

OOSDL::invalid_font::invalid_font(const std::string& fontname)
: fontname(fontname)
{}

std::string OOSDL::invalid_font::getFontname() const
{
	return this->fontname;
}

const char* OOSDL::invalid_font::what() const throw()
{
	return std::string("Fontname: ").append(this->getFontname()).c_str();
}

#endif
