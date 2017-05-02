#ifndef OOSDL_Sound_HPP
#define OOSDL_Sound_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "OOSDL_CONFIG.hpp"
#include "interrupted_while_waiting.hpp"

/**
 * Sound representa um efeito sonoro em execução e possui métodos para controlar
 * a execução deste efeito sonoro. Sound possui uma máquina de estado que define
 * três estados: Playing, Paused e Stopped.
 * Playing: Este é o estado inicial. O som está atualmente em execução.
 * Paused: Durante a execução do som, ele foi pausado. Neste estado o som não
 * 		está mais tocando mas seus recursos não foram liberados, logo, é
 * 		possível voltar para Playing ou ir para Stopped.
 * Stopped: Este é o estado final. O som não está mais em execução e já teve
 * 		seus recursos liberados, logo, ele não pode voltar a tocar novamente. O
 * 		som alcança este estado automaticamente quando sua execução termina, ou
 * 		quando ativado pelo cliente.
 */

namespace OOSDL {
	class Sound {
	public:
		enum class State { 
			Playing,
			Paused,
			Stopped
		};
		
	private:
		Mix_Chunk* chunk;
		int channel;
		State currentState;
		bool looping;
	protected:
		virtual void setState(State newState);									// Muda o estado do objeto.
	public:
		
		Sound(Mix_Chunk* chunk, int channel, bool loops, State currentState);
		virtual ~Sound();
		
		// Controle de execução.
		virtual void stop();													// Encerra a execução do som.			// Playing, Paused -> Stopped;
		virtual void pause();													// Se estava tocando, pára de tocar.	// Playing -> Paused;
		virtual void resume();													// Se estava pausado, volta a tocar.	// Paused -> Playing;
		virtual State wait() const throw (interrupted_while_waiting);			// Espera o som parar sua execução(seja por ter sido pausado, parado ou encerrado normalmente). Levanta exceção se a thread corrente receber uma interrupção. Retorna o estado do som no momento do retorno.
		
		// Métodos de verificação.
		virtual State getState() const;											// Verifica o estado atual do som.
		virtual bool inState(State state) const;								// Verifica se o objeto está em determinado estado. (equivalente à "this->getState() == state")
		virtual bool isLooping() const;											// Verifica se o som está executando em loop.
		
		// Controle de volume.
		virtual void setVolume(Uint32 volume = OOSDL_AUDIO_MAX_VOLUME);
		virtual Uint32 getVolume() const;
		
		// Métodos SDL.
		virtual const Mix_Chunk* get_Mix_Chunk() const;
		virtual Mix_Chunk* get_Mix_Chunk();
		virtual Uint32 get_Mix_Channel() const;
	};
};

#endif
