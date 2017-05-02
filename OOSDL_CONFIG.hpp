#ifndef OOSDL_CONFIG
#define OOSDL_CONFIG

#define OOSDL_WINDOW_DEFAULT_WIDTH			640
#define OOSDL_WINDOW_DEFAULT_HEIGHT			480
#define OOSDL_WINDOW_DEFAULT_FPS			40									// Número de máximo de frames desenhados por segundo.
#define OOSDL_WINDOW_DEFAULT_FULLSCREEN		false								// Determina se, por padrão, a aplicação será fullscreen. (true ou 1 para verdadeiro; e false ou 0 para falso)

#define OOSDL_WINDOW_BITSPERPIXEL			32
#define OOSDL_WINDOW_VIDEOFLAGS				(SDL_HWSURFACE|SDL_DOUBLEBUF)

#define OOSDL_BITMAP_BYTESPERPIXEL			(OOSDL_WINDOW_BITSPERPIXEL/8)

#define OOSDL_FONT_DEFAULT_SIZE				12									// Tamanho padrão para fontes.
#define OOSDL_FONT_DEFAULT_NAME				"UbuntuMono-R.ttf"					// Fonte padrão.
#define OOSDL_FONT_DEFAULT_COLOR			Color::White						// Cor padrão para fontes.

#define OOSDL_SDL_INITFLAGS					SDL_INIT_EVERYTHING					// Flags de inicialização SDL

#define OOSDL_AUDIO_MIXFLAGS				0b11111111111111111111111111111111	// Flags Mix que especificam quais formatos de audio serão suportados.
#define OOSDL_AUDIO_MIN_CHANNELS			4									// Número inicial de canais de audio.
#define OOSDL_AUDIO_MAX_CHANNELS			16									// Número máximo de canais de audio.
#define OOSDL_AUDIO_MAX_VOLUME				100									// O range do volume para o usuário setar vai de zero até este número; sendo zero mudo e este número o volume máximo. Deve caber num Uint32.

#define OOSDL_AUDIO_DEFAULT_FREQUENCY		22050								// 44100(44.1KHz) | 22050(22.05KHz)
#define OOSDL_AUDIO_DEFAULT_FORMAT			AUDIO_S16SYS						// AUDIO_S16SYS(signed 16 bits)
#define OOSDL_AUDIO_DEFAULT_N_CHANNELS		2									// 1(mono) | 2(stereo)
#define OOSDL_AUDIO_DEFAULT_CHUNKSIZE		1024								// 512 | 1024 | 2048 | 4096

#define OOSDL_SOUND_INTERN_WAITING_DELAY	10
#define OOSDL_MUSIC_INTERN_WAITING_DELAY	100

#define OOSDL_THREAD_DEFAULT_THREADNAME		"Current Thread"

#define OOSDL_DEBUG_ENABLED					0									// Se 0(zero; false), desabilita o debug em tempo de compilação, não alocando nenhum tipo de recurso para deputação.

#define OOSDL_SPRITE_TEXT_SETX_INVALIDA 	"Nova posição X inválida."
#define OOSDL_SPRITE_TEXT_SETY_INVALIDA 	"Nova posição Y inválida."
#define OOSDL_SPRITE_TEXT_CONS_INVALIDA 	"Sprite não pôde ser criado devido a coordenadas inválidas."

#endif
