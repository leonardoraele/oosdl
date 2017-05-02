#ifndef OOSDL_exception_CPP
#define OOSDL_exception_CPP

#include "exception.hpp"

template<char* (*function)()>
OOSDL::exception<function>::exception()
: message(function())
{}

template<char* (*function)()>
OOSDL::exception<function>::exception(std::string message)
: message(message)
{}

template<char* (*function)()>
const char* OOSDL::exception<function>::what() const throw()
{
	return this->message.c_str();
}

#endif
