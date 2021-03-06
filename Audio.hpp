#ifndef OOSDL_Audio_HPP
#define OOSDL_Audio_HPP

#include "OOSDL_CONFIG.hpp"
#include <string>
#include <map>
#include <stdexcept>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "MixerClient.hpp"
#include "Sound.hpp"
#include "Music.hpp"
#include "no_channel_found.hpp"
#include "mix_error.hpp"

/**
 * A classe Audio provém um conjunto de funções para lidar com audio de maneira
 * simples. As funções contidas pela classe Audio são estáticas e devem ser
 * chamadas como funções globais. O motivo de Audio ser uma classe e não um
 * namespace é o encapsulamento dos dados de audio. Audio é uma classe que nunca
 * deve ser instanciada(Audio se protege contra instanciação não possuindo
 * nenhum método público para tal - com construtores e afins - e também não
 * define dados de instância, fazendo com que uma instanciação, mesmo que fosse
 * possível, seja inútil).
 */

namespace OOSDL {
	class Audio {
	public:
		enum class Frequency : int { None = 0, _default = OOSDL_AUDIO_DEFAULT_FREQUENCY	// Limita as opções de frequência de audio.
			, _44100KHz = 44100
			, _22050KHz = 22050
		};
		enum class Format : Uint16 { None = 0, _default = OOSDL_AUDIO_DEFAULT_FORMAT		// Define as opções de formato de audio. Default is s16bits_sys.
			, u8bits			= AUDIO_U8										// unsigned 8 bits
			, s8bits			= AUDIO_S8										// signed 8 bits
			, u16bits_lilendian	= AUDIO_U16LSB									// unsigned 16 bits - little endian
			, u16bits_bigendian	= AUDIO_U16MSB									// unsigned 16 bits - big endian
			, u16bits_sys		= AUDIO_U16										// unsigned 16 bits - system defined
			, s16bits_lilendian	= AUDIO_S16LSB									// signed 16 bits - little endian
			, s16bits_bigendian	= AUDIO_S16MSB									// signed 16 bits - big endian
			, s16bits_sys		= AUDIO_S16										// signed 16 bits - system defined
		};
		
	private:
		// Atributos de configuração.
		int n_channels;
		int chunksize;
		Frequency frequency;
		Format format;
		
		// Gerência de canais.
		int getEmptyChannel() throw (no_channel_found);							// Não é contante porque além de procurar por canais, pode realocar novos canais ou desalocar canais desnecessários.
		
		// Cache dos arquivos de audio.
		std::map<std::string, Mix_Chunk*> chunk_cache;
		std::map<std::string, Mix_Music*> music_cache;
		
		// Controle de instância.
		static Audio instance;
		Audio(
				int n_channels = OOSDL_AUDIO_DEFAULT_N_CHANNELS,
				int chunksize = OOSDL_AUDIO_DEFAULT_CHUNKSIZE,
				Frequency frequency = Frequency::_default,
				Format format = Format::_default
			);
	public:
		
		// Controle de instância.
		~Audio();
		static Audio& getInstance();
		
		// Configurações:
		void reset();															// Chama reset com argumentos 0 e None. Isto não irá mudar nenhuma configuração de audio, mas irá liberar todos os recursos que estiverem sendo mantidos em cache.
		void reset(Frequency frequency, Format format, int n_channels, int chunksize);
																				// Reseta o audio com novas configurações. Use 0(zero) ou None nos parâmetros que não deseja modificar.
		int getNumberOfChannels() const;										// Recupera o número de canais(saídas) de audio disponíveis. (1: mono, 2: stereo)
		Format getFormat() const;												// Recupera o formato do audio. (8 bits, 16 bits)
		int getChunkSize() const;												// Recupera o chunksize.
		Frequency getFrequency() const;											// Recupera a frequência do audio.
		
		// Efeitos Sonoros:
		Sound playSound(std::string filename, Uint32 volume = OOSDL_AUDIO_MAX_VOLUME, bool loop = false)
																				// Executa um arquivo de audio. O volume é de 0 a 100. loop define se o som ficará se repetindo até que seja parado.
																				// Retorna um objeto Sound que representa a execução deste som para que ele seja controlado. O som pára a execução
																				// Os recursos são liberados automaticamente quando o som termina a execução e o objeto Sound é destruído. Se loop for true, os recursos só são liberados após a chamada de stop e a destruição do objeto Sound.
				throw (std::invalid_argument, no_channel_found);				// Levanta invalid_argument se não encontrar o arquivo de audio e no_channel_found e não houverem mais canais de audio livres.
		
		// Música:
		Music playMusic(std::string filename, Uint32 volume = OOSDL_AUDIO_MAX_VOLUME, bool loop = true, int fadein = 0)
																				// Executa uma música no formato WAVE, MOD, MIDI, OGG, MP3, FLAC, etc. O volume é de 0 a 100. Se loop for true, a música fica se repetindo até que seja parada. fadein define um tempo (em milisegundos) para que a música alcance o som definido a partir do mudo; se for 0(zero), a música já inicia no volume especificado.
																				// Retorna um objeto Music que representa a execução da música. Se loop for true, a música só pára quando o método stop for chamado e os recursos só são liberados quando o objeto é destruído.
				throw (std::invalid_argument, mix_error);						// Levanta invalid_argument se não encontrar o arquivo de audio.
		
	};
};
#endif
