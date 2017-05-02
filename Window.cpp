#ifndef OOSDL_Window_CPP
#define OOSDL_Window_CPP

#include "Window.hpp"
#include <SDL/SDL.h>
#include <algorithm>
#include <exception>
#include "SDLClient.cpp"
#include "Debug.cpp"
#include "OOSDL_CONFIG.hpp"

SDL_Surface* OOSDL::Window::createWindow(int width, int height, bool fullscreen)
throw (sdl_error)
{
	SDL_Surface* result = SDL_SetVideoMode(width, height, OOSDL_WINDOW_BITSPERPIXEL, OOSDL_WINDOW_VIDEOFLAGS|( SDL_FULLSCREEN * fullscreen ));
	
	if (!result)
	{
		throw sdl_error();
	}
	
	return result;
}

OOSDL::Window::Window(Uint16 width, Uint16 height, Uint16 fps, bool fullscreen)
throw (sdl_error)
: fps(fps), fullscreen(fullscreen), Bitmap(NULL), SDLClient(OOSDL_SDL_INITFLAGS)
{
	// Inicializa o surface
	SDL_Surface* window = this->createWindow(width, height, this->fullscreen);
	this->set_SDL_Surface(window);
}

OOSDL::Window::~Window()
{}

Uint16 OOSDL::Window::getFPS() const
{
	return this->fps;
}

void OOSDL::Window::setFPS(Uint16 fps)
{
	this->fps = fps;
}

void OOSDL::Window::update()
throw (std::bad_exception)
{
	static Uint32 last_update = 0;
	
	// Desenha a tela. (realiza a atualização propriamente dita)
	SDL_Flip(this->get_SDL_Surface());
	
	// Procura na pilha de eventos por SDL_Quit.
	SDL_Event event;
	SDL_PumpEvents();
	if (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_QUITMASK) > 0)
	{
		this->quit();
	}
	
	// Controle de velocidade de atualização:
	int time_limit = 1000/this->fps;					// Calcula o limite de tempo, em milisegundos, de cada atualização para manter o FPS desejado. (as atualizações devem demorar menos que este número tempo ou a tela demorará mais que um segundo para ser atualizada "FPS" vezes)
	int work_time = SDL_GetTicks() - last_update;		// Calcula o tempo gasto, em milisegundos, para realizar a última atualização. (diferença entre o tempo atual e o da última chamada de update) OBS: Considera-se que o jogo está num loop que chama update a cada interação.
	if (work_time < time_limit)							// A máquina deve esperar o tempo restante para se completar o time_limit, para que a tela seja atualizada exatamente "FPS" vezes em um segundo.
	{
		SDL_Delay(time_limit - work_time);								// A condição antes da chamada de SDL_Delay deve existir porque esta função não aceita números negativos ou zero.
	}
	
	// Limpa a tela para que ela seja redesenhada na próxima atualização.
	this->clear();
	
	// Prepara a variável para calcular corretamente o tempo da próxima atualização.
	// A atualização termina, então last_update passa a ser o tempo do término desta atualização(a mais recente realizada).
	last_update = SDL_GetTicks();
}

void OOSDL::Window::changeResolution(Uint16 width, Uint16 height)
{
	// Cria uma nova surfaace com as resoluções desejadas.
	// Se a criação for bem sucedida, usa-se o novo surface, se não... TODO: Deveria ser levantada uma exceção.
	SDL_Surface* newWindow = this->createWindow(width, height, this->isFullscreen());
	if (newWindow != NULL)
	{
		this->set_SDL_Surface(newWindow);
	}
	else
	{
		// TODO Tratar.
	}
}

bool OOSDL::Window::isFullscreen() const
{
	return this->fullscreen;
}

void OOSDL::Window::setFullscreen(bool fullscreen)
{
	if (this->isFullscreen() != fullscreen)
	{
		SDL_WM_ToggleFullScreen(this->get_SDL_Surface());
		this->fullscreen = !this->fullscreen;
	}
}

bool OOSDL::Window::toggleFullscreen()
{
	this->setFullscreen(!this->isFullscreen());
	return this->isFullscreen();
}

void OOSDL::Window::quit()
{
	SDL_FreeSurface(this->get_SDL_Surface());
	this->set_SDL_Surface(NULL);
}

bool OOSDL::Window::minimize() const
{
	return SDL_WM_IconifyWindow();
}

bool OOSDL::Window::getGrabMode() const
{
	return SDL_WM_GrabInput(SDL_GRAB_QUERY);
}

void OOSDL::Window::setGrabMode(bool b)
{
	b ?
		SDL_WM_GrabInput(SDL_GRAB_ON)
		:
		SDL_WM_GrabInput(SDL_GRAB_OFF)
		;
}

void OOSDL::Window::setIcon(const Bitmap& icon)
{
	Bitmap* unconstant_icon = const_cast<Bitmap*>(&icon);
	SDL_WM_SetIcon((*unconstant_icon).get_SDL_Surface(), NULL);
}

void OOSDL::Window::dispose()
{
	Bitmap::dispose();
}

#endif
