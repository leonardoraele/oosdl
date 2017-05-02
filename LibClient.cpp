#ifndef OOSDL_LibClient_CPP
#define OOSDL_LibClient_CPP

#include "LibClient.hpp"
#include "sdl_error.cpp"

template <typename init_return_t, typename init_arg_t,
		init_return_t expected_return, init_return_t (*init)(init_arg_t), void (*free)(void)>
	int OOSDL::LibClient<init_return_t, init_arg_t, expected_return, init, free>
::n_initializes = 0;

template <typename init_return_t, typename init_arg_t,
		init_return_t expected_return, init_return_t (*init)(init_arg_t), void (*free)(void)>
		OOSDL::LibClient<init_return_t, init_arg_t, expected_return, init, free>
::LibClient(init_arg_t arg)
throw (sdl_error)
{
	if (n_initializes == 0)
	{
		init_return_t result = init(arg);
		if (result != success_result)
		{
			throw sdl_error();
		}
	}
	
	n_initializes++;
}

template <typename init_return_t, typename init_arg_t,
		init_return_t expected_return, init_return_t (*init)(init_arg_t), void (*free)(void)>
		OOSDL::LibClient<init_return_t, init_arg_t, expected_return, init, free>
::~LibClient()
{
	n_initializes--;
	
	if (n_initializes == 0)
	{
		free();
	}
}

#endif
