#ifndef OOSDL_exception_HPP
#define OOSDL_exception_HPP

#include <exception>
#include <string>

namespace OOSDL {
	char* empty_string();
	
	template<char* (*function)() = empty_string>
	class exception : std::exception {
		const std::string message;
	public:
		explicit exception();
		explicit exception(std::string);
		virtual ~exception() throw() {};
		
		virtual const char* what() const throw();
	};
	
	char* empty_string()
	{
		 static char empty[1] = "";
		 
		 return empty;
	}
};

#endif
