#ifndef OOSDL_Music_CPP
#define OOSDL_Music_CPP

#include "Music.hpp"
#include <SDL/SDL.h>
#include "OOSDL_CONFIG.hpp"
#include "Thread.cpp"

OOSDL::Music::Music(Mix_Music* music, State state, bool loops)
: music(music), loop(loops), currentState(state)
{}

OOSDL::Music::~Music()
{}

void OOSDL::Music::setState(State state)
{
	this->currentState = state;
}

void OOSDL::Music::stop(int fadeout)
{
	if (!this->inState(State::Stopped))
	{
		if (fadeout)
		{
			Mix_FadeOutMusic(fadeout);
		}
		else
		{
			Mix_HaltMusic();
		}
		
		this->setState(State::Stopped);
	}
}

void OOSDL::Music::pause()
{
	if (!this->inState(State::Paused))
	{
		Mix_PauseMusic();
		this->setState(State::Paused);
	}
}

void OOSDL::Music::resume()
{
	if (!this->inState(State::Playing))
	{
		Mix_ResumeMusic();
		this->setState(State::Playing);
	}
}

OOSDL::Music::State OOSDL::Music::wait() const
throw (interrupted_while_waiting)
{
	// TODO Mudar esta implementação para usar a função Mix_HookMusicFinished
	static const int delay = OOSDL_MUSIC_INTERN_WAITING_DELAY;
	Thread currentThread = Thread::getCurrentThread();
	
	while (Mix_PlayingMusic())
	{
		if (currentThread.wasInterrupted())
		{
			throw interrupted_while_waiting();
		}
		
		SDL_Delay(delay);
	}
}

std::string OOSDL::Music::getStateText(State state)
{
	// TODO Evitar switches. Procurar uma implementação melhor para os estados.
	std::string result;
	switch (state)
	{
		case State::Playing:
			result = "Playing";
			break;
		case State::Paused:
			result = "Paused";
			break;
		case State::Stopped:
			result = "Stopped";
			break;
		case State::Fading:
			result = "Fading";
			break;
		default:
			result = "Unknown";
			break;
	}
	return result;
}

std::string OOSDL::Music::getStateText() const
{
	return OOSDL::Music::getStateText(this->getState());
}

OOSDL::Music::State OOSDL::Music::getState() const
{
	return this->currentState;
}

bool OOSDL::Music::inState(State state) const
{
	return this->getState() == state;
}

OOSDL::Music::FadingState OOSDL::Music::getFadingState() const
{
	FadingState result;
	
	switch (Mix_FadingMusic())
	{
		case MIX_NO_FADING:
			result = FadingState::None;
			break;
		case MIX_FADING_OUT:
			result = FadingState::Out;
			break;
		case MIX_FADING_IN:
			result = FadingState::In;
			break;
	}
	
	return result;
}

bool OOSDL::Music::inFadingState(FadingState fadingState) const
{
	return this->getFadingState() == fadingState;
}

bool OOSDL::Music::isLooping() const
{
	return this->loop;
}

void OOSDL::Music::setVolume(Uint32 volume)
{
	// Formata o volume de OOSDL_AUDIO_MAX_VOLUME para MIX_MAX_VOLUME.
	volume = ((volume*MIX_MAX_VOLUME)/OOSDL_AUDIO_MAX_VOLUME);
	
	// Altera o volume.
	Mix_VolumeMusic(volume);
}

Uint32 OOSDL::Music::getVolume() const
{
	// Verifica o volume.
	Uint32 volume = Mix_VolumeMusic(-1);
	
	// Formata o volume de MIX_MAX_VOLUME para OOSDL_AUDIO_MAX_VOLUME.
	volume = (volume * OOSDL_AUDIO_MAX_VOLUME)/MIX_MAX_VOLUME;
	
	// Retorna
	return volume;
}

const Mix_Music* OOSDL::Music::get_Mix_Music() const
{
	// Transforma this em um ponteiro não constante.
	const Music* unconst_this = const_cast<Music*>(this);
	
	// Chama a versão não constante deste método.
	return unconst_this->get_Mix_Music();
}

Mix_Music* OOSDL::Music::get_Mix_Music()
{
	return this->music;
}

#endif
