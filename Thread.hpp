#ifndef OOSDL_Thread_HPP
#define OOSDL_Thread_HPP

#include <SDL/SDL.h>
#include <string>
#include <map>
#include "sdl_error.hpp"
#include "Runnable.hpp"
#include "thread_not_running.hpp"
#include "time_limit_exceed.hpp"
#include "interrupted_while_waiting.hpp"

/*
TODO:
O mapa estático threads deveria ser mapa de < Uint32, std::pait<SDL_Thread*,
bool> >. Este booleano irá determinar quando a thread correspontente ao par foi
interrompida. A flag de interrupção não pode ficar na classe Thread porque ela é
meramente uma classe gerenciadora de cerursos. Quando Thread for copiada,
existirão vários objetos Thread que devem corresponder à mesma thread do
sistema. Mas quando um objeto Thread é copiado, atributos relativos ao objeto
real thread(o gerenciado, SDL_Thread) são duplicados(flag de interrupção).
Assim, se um objeto thread é copiado e um dos dois é chamado interrupt() o outro
deve ser alterado também; logo, conclui-se que a flag de interrupção deve estar
associada diretamente ao objeto real gerenciado SDL_Thread e não ao objeto
gerenciador de recursos OOSDL::Thread.
TODO:
O nome da thread também deve ser estático; não é o nome do gerenciador de
recursos e sim o nome da thread em si.
*/

namespace OOSDL {
	class Thread {
		int (*function)(void*);
		Runnable* runnable;
		SDL_Thread* thread;
		bool interrupt_flag;
		Uint32 id;
		std::string threadName;
		
		static std::map< Uint32, SDL_Thread* > threads;							// Associa cada objeto thread ao seu id.
		static int call_runnable(void*);										// Função executada por start quando houver um runnable. Chama o método run do runnable.
		static int waitThread(void*);											// Comportamento da thread de espera. (utilizada no método wait(Uint64))
		
		Uint32 getId() const;													// Recupera um identificador único da thread.
		Thread(SDL_Thread*);													// Usado internamente para construir um novo gerenciador de recursos de uma determinada thread.
	public:
		explicit Thread(int (*function)(void*), const std::string& threadName = "");	// Cria uma thread que será iniciada a partir de uma função.
		explicit Thread(Runnable*, const std::string& threadName = "");					// Cria uma thread que será iniciada a partir da função run do objeto runnable.
		~Thread();
		
		// Operadores:
		bool operator==(const Thread& rhs) const;
		
		// Métodos de verificação:
		std::string getName() const;											// Recupera o nome atribuído à esta thread.
		bool isCurrentThread() const;											// Verifica se este objeto thread representa a atual thread em execução.
		bool isRunning() const;													// Verifica se a thread atual está sendo executada no atual momento.
		
		// Recuperar a thread corrente:
		static Thread getCurrentThread();										// Recupera a instância de Thread que representa a atual thread em execução.
		
		// Métodos de sinalização.
		void interrupt();														// Envia uma sinalização que fica visível para a thread em execução; esta decide como reagir.
		bool wasInterrupted() const;											// Verifica se uma sinalização já foi enviada para esta thread através do método interrupt().
		
		// Métodos de controle da execução da thread.
		void start(void*)														// Inicia a execução da thread. Envia um ponteiro como parâmetro para a nova thread.
				throw (sdl_error);
		int wait(Uint32 timeout)												// Espera até que a thread se encerre e retorna o retorno da função que a thread executa.
				throw (															// Levanta exceção nas seguintes situações:
						thread_not_running,										// Caso a thread não esteja sendo executada durante a chama deste método. Esta exceção pode ser evitada fazendo a devida verificação com o método isRunning().
						time_limit_exceed,										// Caso a espera dure mais que timeout milisegundos. Se timeout for zero, está exceção nunca é levantada.
						interrupted_while_waiting								// Caso esta thread receba uma interrupção durante a espera. O tempo restante de espera pode ser recuperado através da exceção.
					);
		void finish();															// Finaliza a execução da thread. O uso deste método é altamente desencorajado por questões de segurança e consistência do programa.
		static void delay(Uint32 ms);											// Pára a execução da thread corrente por ms milisegundos.
		
		// Métodos SDL:
		SDL_Thread* get_SDL_Thread();											// Recupera um ponteiro para o objeto SDL que representa a thread.
		const SDL_Thread* get_SDL_Thread() const;								// Versão constante de get_SDL_Thread().
		
	};
};
#endif
