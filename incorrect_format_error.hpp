#ifndef OOSDL_incorrect_format_error_HPP
#define OOSDL_incorrect_format_error_HPP

#include <cstddef>
#include <exception>
#include <sstream>

namespace OOSDL {
	class incorrect_format_error : std::exception {
		const char* expected;
		const char* got;
	public:
		explicit incorrect_format_error(const char* expected = NULL, const char* got = NULL);
		virtual ~incorrect_format_error() throw () {};
		
		virtual const char* expected_regex() const;
		virtual const char* unformatted_string() const;
		
		virtual const char* what() const throw();
	};
};

#endif
