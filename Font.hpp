#ifndef OOSDL_Font_HPP
#define OOSDL_Font_HPP

#include <string>

#include <SDL/SDL_ttf.h>
#include "OOSDL_CONFIG.hpp"
#include "Color.hpp"
#include "invalid_font.hpp"

namespace OOSDL {
	class Font {
		int size;
		std::string fontname;
		Color color;
		int style;
		TTF_Font* ttf_font;
		static const Font defaultFont;
	public:
		Font(); // Cria uma fonte como cópia da fonte padrão.
		Font(int size, const std::string& fontname, const Color& color = OOSDL_FONT_DEFAULT_COLOR,
				int style = TTF_STYLE_NORMAL) throw (invalid_font);
		Font(const Font& rhs);
		virtual ~Font();
		virtual Font& operator=(const Font&);
		
		// Default font
		static const Font& getDefaultFont();
		
		// Main font attributes:
		virtual int getSize() const;
		virtual const std::string& getName() const;
		virtual const Color& getColor() const;
		
		virtual void setSize(int size);
		virtual void setColor(const Color& color);
		
		// Style settings:
		virtual bool isBold() const;
		virtual bool isItalic() const;
		virtual bool isUnderlined() const;
		virtual bool isStriked() const;
		
		virtual void setBold(bool bold);
		virtual void setItalic(bool italic);
		virtual void setUnderlined(bool underlined);
		virtual void setStriked(bool striked);
		
		// Rendering metrics:
		virtual int getTextWidth(const std::string& text) const;
		virtual int getTextHeight() const;
		
		// SDL access methods:
		virtual int getSDLStyleSettings() const;
		virtual void setSDLStyleSettings(int settings);
		
		virtual TTF_Font* to_TTF_Font();
		virtual const TTF_Font* to_TTF_Font() const;
	};
};
#endif
