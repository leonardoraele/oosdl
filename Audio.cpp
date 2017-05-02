#ifndef OOSDL_Audio_CPP
#define OOSDL_Audio_CPP

#include "Audio.hpp"
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>
#include <stdexcept>
#include "OOSDL_CONFIG.hpp"
#include "MixerClient.cpp"
#include "Lock.cpp"
#include "Music.cpp"
#include "Sound.cpp"
#include "no_channel_found.cpp"
#include "mix_error.cpp"

OOSDL::Audio OOSDL::Audio::instance = OOSDL::Audio();

OOSDL::Audio::Audio(int n_channels, int chunksize, Frequency frequency, Format format)
: n_channels(0), chunksize(0), frequency(Frequency::None), format(Format::None)
{
	this->reset(frequency, format, n_channels, chunksize);
}

OOSDL::Audio& OOSDL::Audio::getInstance()
{
	return instance;
}

OOSDL::Audio::~Audio()
{
	Mix_CloseAudio();
}

void OOSDL::Audio::reset()
{
	this->reset(Frequency::None, Format::None, 0, 0);
}

void OOSDL::Audio::reset(Frequency frequency, Format format, int n_channels, int chunksize)
{
	// Prepara as execuções atuais de audio para o reset.
	{
		// Pára a execução de todos os canais.
		for (int i = 0; i < this->getNumberOfChannels(); i++)
		{
			Mix_HaltChannel(i);
		}
	
		// Libera todos os sons em cache.
		for (auto iter = chunk_cache.begin(); iter != chunk_cache.end(); iter++)
		{
			Mix_FreeChunk(iter->second);
			chunk_cache.erase(iter);
		}
		
		// Pára a execução de música.
		Mix_HaltMusic();
		
		// Libera todas as músicas em cache.
		for (auto iter = music_cache.begin(); iter != music_cache.end(); iter++)
		{
			Mix_FreeMusic(iter->second);
			music_cache.erase(iter);
		}
	}
	
	// Seta os novos atributos.
	if (frequency	!= Frequency::None)		this->frequency		= frequency;
	if (format		!= Format::None)		this->format		= format;
	if (n_channels	!= 0)					this->n_channels	= n_channels;
	if (chunksize	!= 0)					this->chunksize		= chunksize;
	
	// Realoca o número correto de canais.
	Mix_AllocateChannels(this->getNumberOfChannels());
	
	// Reseta as configurações de audio.
	Mix_CloseAudio();
	Mix_OpenAudio(
			(int) this->frequency,
			(Uint16) this->format,
			this->n_channels,
			this->chunksize
		);
}

int OOSDL::Audio::getNumberOfChannels() const
{
	return this->n_channels;
}

OOSDL::Audio::Format OOSDL::Audio::getFormat() const
{
	return this->format;
}

int OOSDL::Audio::getChunkSize() const
{
	return this->chunksize;
}

OOSDL::Audio::Frequency OOSDL::Audio::getFrequency() const
{
	return this->frequency;
}

int OOSDL::Audio::getEmptyChannel()
throw (no_channel_found)
{
	static int currentChannel = 0; // circular fit
	
	// Tranca o objeto para evitar concorrência.
	Lock(this);
	
	// Varre todos os canais procurando um livre.
	for (int i = 0; !Mix_Playing(currentChannel) && i < this->getNumberOfChannels(); i++)
	{
		currentChannel = (currentChannel + 1) % this->getNumberOfChannels();
	}
	
	// Se não encontrou um canal livre, aloca mais canais.
	if (Mix_Playing(currentChannel))
	{
		// Verifica se há espaço para alocar mais canais.
		if (this->getNumberOfChannels() == OOSDL_AUDIO_MAX_CHANNELS)			// Verifica se está no limite do alocamento.
		{
			throw no_channel_found();											// TODO Tratar corretamente.
		}
		
		// Há espaço, aloca mais canais.
		Mix_AllocateChannels(OOSDL_AUDIO_MAX_CHANNELS);							// TODO Deveria alocar de pouco.
		currentChannel = this->getNumberOfChannels() - 1;						// Facilita a busca.
		currentChannel = this->getEmptyChannel();								// Busca por um vazio.
	}
	
	return currentChannel;
}

OOSDL::Sound OOSDL::Audio::playSound(std::string filename, Uint32 volume, bool loop)
throw (std::invalid_argument, no_channel_found)
{
	Mix_Chunk* sample;
	
	// Recupera o arquivo de audio do cache.
	if (this->chunk_cache.find(filename) == this->chunk_cache.end())						// Se não houver um carregado no cache.
	{
		sample = Mix_LoadWAV(filename.c_str());									// Carrega o arquivo.
		if (!sample)
		{
			throw std::invalid_argument(Mix_GetError());						// Levanta excessão se houve erro. TODO Tratar corretamente.
		}
		else
		{
			this->chunk_cache[filename] = sample;										// É adicionado no cache somente se não houve nenhum erro.
		}
	}
	// Se ele já existir no cache, é carregado.
	else
	{
		sample = this->chunk_cache[filename];
	}
	
	// Escolhe o canal.
	int channel = this->getEmptyChannel();
	
	// Configura o volume.
	Mix_Volume(channel, (volume * MIX_MAX_VOLUME) / OOSDL_AUDIO_MAX_VOLUME);
	
	// Toca a música.
	int playing = Mix_PlayChannel(channel, sample, loop ? -1 : 0);				// Toca o som.
	Sound::State state = playing ? Sound::State::Playing : Sound::State::Stopped;	// Define o estado correto.
	Sound result(sample, channel, loop, state);									// Cria o objeto de retorno.
	
	return result;
}

OOSDL::Music OOSDL::Audio::playMusic(std::string filename, Uint32 volume, bool loop, int fadein)
throw (std::invalid_argument, mix_error)
{
	Mix_Music* music;
	Music::State initialState = Music::State::None;
	
	// Carrega a música.
	if (this->music_cache.find(filename) == music_cache.end())
	{
		music = Mix_LoadMUS(filename.c_str());
		if (!music)
		{
			throw std::invalid_argument(Mix_GetError());	// TODO
		}
		else
		{
			music_cache[filename] = music;
		}
	}
	else
	{
		music = music_cache[filename];
	}
	
	// Ajusta o volume.
	Mix_VolumeMusic((volume * MIX_MAX_VOLUME) / OOSDL_AUDIO_MAX_VOLUME);
	
	// Toca a música
	bool success = false;
	if (fadein)
	{
		success = !Mix_FadeInMusic(music, loop ? -1 : 1, fadein);
		initialState = Music::State::Fading;
	}
	else
	{
		success = !Mix_PlayMusic(music, loop ? -1 : 1);
		initialState = Music::State::Playing;
	}
	
	// Verifica por erros.
	if (!success)
	{
		throw mix_error();
		initialState = Music::State::Stopped;
	}
	
	return Music(music, initialState, loop);
}

#endif
