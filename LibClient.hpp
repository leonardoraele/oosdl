#ifndef OOSDL_LibClient_HPP
#define OOSDL_LibClient_HPP

#include "sdl_error.hpp"

namespace OOSDL {
	template <
			typename init_return_t,						// <typename> Tipo de retorno da função de inicialização desta biblioteca.
			typename init_arg_t,						// <typename> Argumento que a função de inicialização espera.
			init_return_t expected_return,				// <value>    Valor que a função de inicialização desta biblioteca deve retornar para informar que foi inicializada com sucesso. (do tipo definido no primeiro argumento do template)
			init_return_t (*init)(init_arg_t),			// <function> Ponteiro para função de inicialização desta biblioteca.
			void (*free)(void)							// <function> Ponteiro para função de saída desta biblioteca.
		>
	class LibClient {
		static int n_initializes;
		static const init_return_t success_result = expected_return;
	public:
		LibClient(init_arg_t arg)
				throw (sdl_error);
		virtual ~LibClient();
	};
};

#endif
