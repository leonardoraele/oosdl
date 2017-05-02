#ifndef OOSDL_Debug_HPP
#define OOSDL_Debug_HPP

#include <ostream>
#include "OOSDL_CONFIG.hpp"

namespace OOSDL {
	class Debug {
		#if OOSDL_DEBUG_ENABLED
		bool enabled;
		std::ostream* output;
		#endif
	public:
		explicit Debug(std::ostream* = NULL, bool enabled = false);
		
		void enable();
		void disable();
		bool isEnabled() const;
		
		void setOutputStream(std::ostream*);
		std::ostream* getOutputStream();
		
		template <typename Data>
		Debug& operator<<(const Data& data);
	};
};

#endif
