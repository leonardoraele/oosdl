#ifndef OOSDL_Runnable_HPP
#define OOSDL_Runnable_HPP

namespace OOSDL {
	class Runnable {
	public:
		virtual int run(void*) = 0;
	};
};
#endif
