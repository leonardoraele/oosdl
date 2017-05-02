#ifndef OOSDL_Timer_HPP
#define OOSDL_Timer_HPP

#include <SDL/SDL.h>
#include <string>
#include <map>
#include "Runnable.hpp"

namespace OOSDL {
	class Timer {
		Runnable* runnable;
		Uint32 interval;
		std::string name;
		SDL_TimerID id;
		bool running;
		
		static Uint32 timer_func(Uint32 interval, void* param);
	public:
		Timer(Runnable* runnable, Uint32 interval, std::string name = "");
		Timer(const Timer&);
		virtual ~Timer();
		virtual Timer& operator=(const Timer&);
		
		// Controle do timer.
		virtual void start(void* param, Uint32 delay = 0);						// Inicia o timer. A primeira chamada da função acontece após o delay definido e as chamadas seguintes acontecem a cada intervalo de tempo. (se for zero, a primeira chamada é o mesmo que this->getInterval())
		virtual void cancel();													// Cancela o timer. A função pára de ser chamada. Este método não é de chamada obrigatória: quando o objeto é destruído, o timer é cancelado automaticamente.
		
		// Métodos de verificação.
		virtual bool isScheduled() const;										// Verifica se o timer já foi iniciado.
		virtual Uint32 getInterval() const;										// Verifica o tempo entre cada chamada da função.
		virtual const std::string& getName() const;								// Recupera o nome dado ao timer.
		
		// Métodos estáticos.
		static Uint32 getCurrentTime();									// Recupera o tempo atual, em milisegundos, desde a inicialização do SDL. Pode ser usado como uma referência para um instante de tempo.
		//TODO:// static Uint32 getSystemTime();									// Recupera o tempo atual do sistema.
		
		// Método SDL.
		virtual SDL_TimerID get_SDL_Timer();
		
	};
};

#endif
