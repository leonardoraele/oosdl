#ifndef OOSDL_invalid_font_HPP
#define OOSDL_invalid_font_HPP

#include <exception>
#include <string>

namespace OOSDL {
	class invalid_font : public std::exception {
		std::string fontname;
	public:
		invalid_font(const std::string&);
		~invalid_font() throw() {};
		
		virtual std::string getFontname() const;
		
		virtual const char* what() const throw();
	};
};
#endif
