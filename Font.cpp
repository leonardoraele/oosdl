#ifndef OOSDL_Font_CPP
#define OOSDL_Font_CPP

#include "Font.hpp"

#include <string>
#include <SDL/SDL_ttf.h>
#include "Color.cpp"

const OOSDL::Font OOSDL::Font::defaultFont = OOSDL::Font(OOSDL_FONT_DEFAULT_SIZE, OOSDL_FONT_DEFAULT_NAME);

OOSDL::Font::Font()
: Font(Font::defaultFont)
{}

OOSDL::Font::Font(int size, const std::string& fontname, const Color& color, int style)
throw (invalid_font)
: ttf_font(NULL), size(size), fontname(fontname), color(color), style( style )
{};

OOSDL::Font::Font(const Font& rhs)
: ttf_font(NULL), size(0), fontname(std::string()), color(Color(0)), style(0)
{
	this->operator=(rhs);
}

OOSDL::Font::~Font()
{
	if (this->ttf_font != NULL)
	{
		TTF_CloseFont(this->ttf_font);
	}
}

OOSDL::Font& OOSDL::Font::operator=(const Font& rhs)
{
	if (this->ttf_font != NULL)
	{
		TTF_CloseFont(this->ttf_font);
		this->ttf_font = NULL;
	}
	this->size = rhs.getSize();
	this->fontname = rhs.getName();
	this->color = rhs.getColor();
	this->style = rhs.getSDLStyleSettings();
}

const OOSDL::Font& OOSDL::Font::getDefaultFont()
{
	return defaultFont;
}

int OOSDL::Font::getSize() const
{
	return this->size;
}

const std::string& OOSDL::Font::getName() const
{
	return this->fontname;
}

const OOSDL::Color& OOSDL::Font::getColor() const
{
	return this->color;
}

void OOSDL::Font::setSize(int size)
{
	if (this->ttf_font != NULL)
	{
		TTF_CloseFont(this->ttf_font);
		this->ttf_font = NULL;
	}
	this->size = size;
}

void OOSDL::Font::setColor(const Color& color)
{
	if (this->ttf_font != NULL)
	{
		TTF_CloseFont(this->ttf_font);
		this->ttf_font = NULL;
	}
	this->color = color;
}

bool OOSDL::Font::isBold() const
{
	return this->style & TTF_STYLE_BOLD;
}

bool OOSDL::Font::isItalic() const
{
	return this->style & TTF_STYLE_ITALIC;
}

bool OOSDL::Font::isUnderlined() const
{
	return this->style & TTF_STYLE_UNDERLINE;
}

bool OOSDL::Font::isStriked() const
{
	return this->style & TTF_STYLE_STRIKETHROUGH;
}

void OOSDL::Font::setBold(bool bold)
{
	if (bold)
	{
		this->style = this->style | TTF_STYLE_BOLD;
	}
	else if (this->isBold())
	{
		this->style = this->style ^ TTF_STYLE_BOLD;
	}
}

void OOSDL::Font::setItalic(bool italic)
{
	if (italic)
	{
		this->style = this->style | TTF_STYLE_ITALIC;
	}
	else if (this->isItalic())
	{
		this->style = this->style ^ TTF_STYLE_ITALIC;
	}
}

void OOSDL::Font::setUnderlined(bool underlined)
{
	if (underlined)
	{
		this->style = this->style | TTF_STYLE_UNDERLINE;
	}
	else if (this->isUnderlined())
	{
		this->style = this->style ^ TTF_STYLE_UNDERLINE;
	}
}

void OOSDL::Font::setStriked(bool striked)
{
	if (striked)
	{
		this->style = this->style | TTF_STYLE_STRIKETHROUGH;
	}
	else if (this->isStriked())
	{
		this->style = this->style ^ TTF_STYLE_STRIKETHROUGH;
	}
}

int OOSDL::Font::getTextWidth(const std::string& text) const
{
	int width, height;
	Font* unconst_this = const_cast<Font*>(this);
	TTF_SizeUTF8(unconst_this->to_TTF_Font(), text.c_str(), &width, &height);
	return width;
}

int OOSDL::Font::getTextHeight() const
{
	return TTF_FontHeight(this->to_TTF_Font());
}

int OOSDL::Font::getSDLStyleSettings() const
{
	return this->style;
}

void OOSDL::Font::setSDLStyleSettings(int settings)
{
	this->style = settings;
}

TTF_Font* OOSDL::Font::to_TTF_Font()
{
	if (this->ttf_font == NULL)
	{
		this->ttf_font = TTF_OpenFont(this->getName().c_str(), this->getSize());
	}
	
	if (this->ttf_font != NULL)
	{
		TTF_SetFontStyle(this->ttf_font, this->style);
	}
	
	return this->ttf_font;
}

const TTF_Font* OOSDL::Font::to_TTF_Font() const
{
	Font* unconst_this = const_cast<Font*>(this);
	
	return unconst_this->to_TTF_Font();
}

#endif
