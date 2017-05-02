#ifndef OOSDL_incorrect_format_error_CPP
#define OOSDL_incorrect_format_error_CPP

#define OOSDL_incorrect_format_error_ERROR_MESSAGE_1	"Was expected format ["
#define OOSDL_incorrect_format_error_ERROR_MESSAGE_2	"] but the string received was ["
#define OOSDL_incorrect_format_error_ERROR_MESSAGE_3	"]."

#include "incorrect_format_error.hpp"
#include <sstream>
#include <string>
#include <cstring>

OOSDL::incorrect_format_error::incorrect_format_error(const char* expected, const char* got)
: expected(expected), got(got)
{}

const char* OOSDL::incorrect_format_error::expected_regex() const
{
	return this->expected;
}

const char* OOSDL::incorrect_format_error::unformatted_string() const
{
	return this->got;
}

const char* OOSDL::incorrect_format_error::what() const throw()
{
	std::stringstream builder;
	
	builder << OOSDL_incorrect_format_error_ERROR_MESSAGE_1;
	builder << this->expected_regex();
	builder << OOSDL_incorrect_format_error_ERROR_MESSAGE_2;
	builder << this->unformatted_string();
	builder << OOSDL_incorrect_format_error_ERROR_MESSAGE_3;
	
	return builder.str().c_str();
}


#endif
