#ifndef OOSDL_Music_HPP
#define OOSDL_Music_HPP

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "OOSDL_CONFIG.hpp"

namespace OOSDL {
	class Music {
	public:
		enum class FadingState { None
			, In
			, Out
		};
		
		enum class State { None
			, Playing
			, Paused
			, Stopped
			, Fading
		};
		
	private:
		Mix_Music* music;
		State currentState;
		bool loop;
	protected:
		virtual void setState(State);
	public:
		Music(Mix_Music* music, State state, bool loops = true);
		virtual ~Music();
		
		// Métodos de controle de execução.
		virtual void stop(int fadeout = 0);										// Pára a música após fadeout milisegundos e libera os recursos. Se fadeout for 0(zero), pára a música instrantaneamente.
		virtual void pause();													// Pausa a execução da música sem liberar os recursos. Ela pode continuar a tocar com o método pouse(). Se stop() não for chamado para liberar os recursos, eles serão mantidos até a próxima chamada de Audio::playMusic.
		virtual void resume();													// Continua a execução da música após ela ter sido parada. Este método não faz nada se a música foi parada com stop() ou se ela nunca foi parada com pause().
		virtual State wait() const throw (interrupted_while_waiting);			// Espera o término da execução da música.
		
		// Métodos de verificação.
		static std::string getStateText(State state);						// Gera um texto humanamente legível de descrição de um possível estado.
		virtual std::string getStateText() const;								// Gera um texto humanamente legível de descrição do estado atual da música. Equivalente a: Music::getStateText(this->getState())
		virtual State getState() const;											// Verifica o estado atual da música.
		virtual bool inState(State state) const;								// Verifica se a música está em um determinado estado.
		virtual FadingState getFadingState() const;								// Recupera o subestado de fading da música. (Fading in ou Fading out)
		virtual bool inFadingState(FadingState fadingState) const;				// Verifica se a música está em um determinado subestado de fading.  (Fading in ou Fading out) 
		virtual bool isLooping() const;											// Verifica se a música está em loop.
		
		// Métodos de controle de volume.
		virtual void setVolume(Uint32 volume = OOSDL_AUDIO_MAX_VOLUME);
		virtual Uint32 getVolume() const;
		
		// Métodos SDL.
		virtual const Mix_Music* get_Mix_Music() const;
		virtual Mix_Music* get_Mix_Music();
	};
};

#endif
