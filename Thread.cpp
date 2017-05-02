#ifndef OOSDL_Thread_CPP
#define OOSDL_Thread_CPP

#include "Thread.hpp"
#include <SDL/SDL.h>
#include <map>
#include "OOSDL_CONFIG.hpp"
#include "thread_not_running.cpp"
#include "time_limit_exceed.cpp"
#include "interrupted_while_waiting.cpp"

std::map< Uint32, SDL_Thread* > OOSDL::Thread::threads;

OOSDL::Thread::Thread(int (*function)(void*), const std::string& threadName)
: runnable(NULL), function(function), threadName(threadName), interrupt_flag(false), id(0), thread(NULL)
{}

OOSDL::Thread::Thread(Runnable* runnable, const std::string& threadName)
: runnable(runnable), function(NULL), threadName(threadName), interrupt_flag(false), id(0), thread(NULL)
{}

OOSDL::Thread::Thread(SDL_Thread* sdl_thread)
: runnable(NULL), function(NULL), thread(sdl_thread), interrupt_flag(false), id(SDL_GetThreadID(sdl_thread)), threadName(OOSDL_THREAD_DEFAULT_THREADNAME)
{}

OOSDL::Thread::~Thread()
{}

bool OOSDL::Thread::operator==(const Thread& rhs) const
{
	return this->getId() == rhs.getId();
}

OOSDL::Thread OOSDL::Thread::getCurrentThread()
{
	return Thread(threads[SDL_ThreadID()]);
}

std::string OOSDL::Thread::getName() const
{
	return this->threadName;
}

Uint32 OOSDL::Thread::getId() const
{
	return this->id;
}

bool OOSDL::Thread::isCurrentThread() const
{
	return (this->isRunning() && this->getId() == SDL_ThreadID());
}

bool OOSDL::Thread::isRunning() const
{
	// TODO Confirmar se funciona assim:
	return this->get_SDL_Thread() != NULL;	// Talvez a thread encerre e continue com pointeiro válido.
	// return this->getId() != 0;	// Talvez receba um Id mesmo antes de começar a execução.
}

void OOSDL::Thread::interrupt()
{
	this->interrupt_flag = true;
}

bool OOSDL::Thread::wasInterrupted() const
{
	return this->interrupt_flag;
}

int OOSDL::Thread::call_runnable(void* param)
{
	void** params = (void**) param;
	Runnable* runnable = (Runnable*) params[0];
	param = params[1];
	
	return runnable->run(param);
}

void OOSDL::Thread::start(void* data)
throw (sdl_error)
{
	// Seleciona a função:
	int (*function)(void*);						// Aqui é feita a escolha da função que será chamada
	if (this->runnable != NULL)					// na inicialização da nova thread.
	{
		function = &call_runnable;				// Se houver um runnable, a função dele deve ser usada.
		
		void** params = new void*[2];
		params[0] = this->runnable;
		params[1] = data;
		data = (void*) params;
	}
	else if (this->function != NULL)			// Se não, é verificado se há uma função pronta.
	{											// Se houver, esta é escolhida para ser usada.
		function = this->function;
	}
	
	// Cria a thread:
	this->thread = SDL_CreateThread(function, data);
	
	// Valida a thread criada:
	if (this->thread == NULL)					// Se houver uma falha na inicialização da thread,
	{											// uma exceção SDL é levantada.
		throw sdl_error();
	}
	
	// Se passar da validação, adiciona à tabela estática de threads.
	else
	{
		this->id = SDL_GetThreadID(this->thread);
		threads[this->id] = this->thread;
	}
}

int OOSDL::Thread::waitThread(void* data)
{
	// Recupera os argumentos.
	void** args = (void**) data;
	int* status = (int*) args[0];
	SDL_Thread* thread = (SDL_Thread*) args[1];
	
	// Espera a thread se encerrar.
	SDL_WaitThread(thread, status);
	
	// Retorna o resultado.
	return *status;
}

int OOSDL::Thread::wait(Uint32 timeout)
throw (thread_not_running, time_limit_exceed, interrupted_while_waiting)
{
	// Verifica o estado da thread.
	if (!this->isRunning())
	{
		throw thread_not_running();				// Se não estiver executando, não há o quê esperar, então levanta exceção.
	}
	
	// Declarações.
	int status;																	// Declara a variável de retorno do método.
	Uint32 start_time = SDL_GetTicks();											// Salva o tempo atual para o cálculo do timeout.
	SDL_Thread* waitingThread;													// Cria a thread que realmente ficará esperando esta thread terminar.
	
	// Inicialização da thread de espera.
	void* args[2];																// Prepara os argumentos para passar para waitingThread quando ela for criada.
	args[0] = &status;															// O primeiro argumento é o retorno da thread que irá terminar.
	args[1] = waitingThread;													// O segundo argumento é a thread em si, para esperar.
	waitingThread = SDL_CreateThread(&waitThread, (void*) args);				// Inicializa esta thread para que ela fique esperando esta thread terminar.
	
	// Finalmente, espera a thread encerrar.
	if (waitingThread != NULL)													// Se não houve nenhuma falha anterior mente, e há uma thread esperando o encerramento desta,
	{
		while (/*TODO:*//*isRunning(waitingThread)*/ false)						// Enquanto a thread de espera estiver rodando, continua esperando.
		{																		// Pára de esperar caso esta thread se encerre, e retorna o resultado.
			if (timeout > 0 && SDL_GetTicks() >= start_time + timeout)			// Se o timeout for alcançador,
			{
				SDL_KillThread(waitingThread);									// Mata a thread que espera e
				throw time_limit_exceed();										// Levanta uma exceção para notificar a falha.
			}
			else if (this->wasInterrupted())									// Se receber uma interrupção durante a espera,
			{
				SDL_KillThread(waitingThread);									// Mata a thread que espera e
				throw interrupted_while_waiting();								// Levanta uma exceção para notificar a falha.
			}
		}
	}
	else																		// Se houve um erro durante a inicialização da thread que iria esperar esta,
	{																			// A própria thread corrente deve esperar.
		SDL_WaitThread(this->thread, &status);									// Interrupções não irão interromper esta espera.
	}																			// TODO Uma configuração de compilação deveria decidir o comportamento correto neste caso:
																				// Se seria realmente este ou um alternativo de cancelar a espera.
	
	// Fim da espera.
	this->thread = NULL;
	this->id = 0;
	
	return status;
}

void OOSDL::Thread::finish()
{
	SDL_KillThread(this->get_SDL_Thread());
}

void OOSDL::Thread::delay(Uint32 ms)
{
	SDL_Delay(ms);
}

SDL_Thread* OOSDL::Thread::get_SDL_Thread()
{
	return this->thread;
}

const SDL_Thread* OOSDL::Thread::get_SDL_Thread() const
{
	Thread* unconst_this = const_cast<Thread*>(this);
	
	return unconst_this->thread;
}

#endif
