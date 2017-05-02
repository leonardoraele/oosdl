#ifndef OOSDL_Sound_CPP
#define OOSDL_Sound_CPP

#include "Sound.hpp"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "OOSDL_CONFIG.hpp"
#include "Thread.cpp"
#include "interrupted_while_waiting.cpp"

OOSDL::Sound::Sound(Mix_Chunk* chunk, int channel, bool loops, State currentState)
: chunk(chunk), channel(channel), looping(loops), currentState(currentState)
{}

OOSDL::Sound::~Sound()
{}

void OOSDL::Sound::setState(State newState)
{
	this->currentState = newState;
}

OOSDL::Sound::State OOSDL::Sound::getState() const
{
	return this->currentState;
}

bool OOSDL::Sound::inState(State state) const
{
	return this->getState() == state;
}

void OOSDL::Sound::stop()
{
	if (this->inState(State::Playing) || this->inState(State::Paused))
	{
		Mix_HaltChannel(this->channel);
		this->setState(State::Stopped);
	}
}

void OOSDL::Sound::pause()
{
	if (this->inState(State::Playing))
	{
		Mix_Pause(this->channel);
		this->setState(State::Paused);
	}
}

void OOSDL::Sound::resume()
{
	if (this->inState(State::Paused))
	{
		Mix_Resume(this->channel);
		this->setState(State::Playing);
	}
}

OOSDL::Sound::State OOSDL::Sound::wait() const
throw (interrupted_while_waiting)
{
	// Tempo de espera por verificação de interrupção.
	static int delay = OOSDL_SOUND_INTERN_WAITING_DELAY;
	
	Thread currentThread = Thread::getCurrentThread();
	
	// Fica esperando
	while (this->getState() == State::Playing)
	{
		// Verifica por interrupções.
		if (currentThread.wasInterrupted())
		{
			throw interrupted_while_waiting();	// TODO Estruturar melhor a exceção.
		}
		
		// Espera.
		SDL_Delay(delay);
	}
}

void OOSDL::Sound::setVolume(Uint32 volume)
{
	// Formata o volume de OOSDL_AUDIO_MAX_VOLUME para MIX_MAX_VOLUME.
	volume = ((volume*MIX_MAX_VOLUME)/OOSDL_AUDIO_MAX_VOLUME);
	
	// Altera o volume.
	Mix_Volume(this->channel, volume);
}

Uint32 OOSDL::Sound::getVolume() const
{
	// Verifica o volume.
	Uint32 volume = Mix_Volume(this->channel, -1);
	
	// Formata o volume de MIX_MAX_VOLUME para OOSDL_AUDIO_MAX_VOLUME.
	volume = (volume * OOSDL_AUDIO_MAX_VOLUME)/MIX_MAX_VOLUME;
	
	// Retorna
	return volume;
}

bool OOSDL::Sound::isLooping() const
{
	return this->looping;
}

const Mix_Chunk* OOSDL::Sound::get_Mix_Chunk() const
{
	// Transforma this em um ponteiro não constante.
	Sound* unconst_this = const_cast<Sound*>(this);
	
	// Chama a versão não constante deste método.
	return this->chunk;
}

Mix_Chunk* OOSDL::Sound::get_Mix_Chunk()
{
	return this->chunk;
}

Uint32 OOSDL::Sound::get_Mix_Channel() const
{
	return this->channel;
}

#endif
