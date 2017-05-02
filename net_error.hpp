#ifndef OOSDL_net_error_HPP
#define OOSDL_net_error_HPP

#include "sdl_error.hpp"

namespace OOSDL {
	class net_error : public sdl_error {
	public:
		net_error();
		net_error(const char*);
		~net_error() throw() {};
	};
};

#endif
