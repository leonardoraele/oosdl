#ifndef OOSDL_Window_HPP
#define OOSDL_Window_HPP

#include <SDL/SDL.h>
#include <string>
#include "Bitmap.cpp"
#include "SDLClient.hpp"
#include "Debug.hpp"
#include "sdl_error.cpp"
#include "OOSDL_CONFIG.hpp"

namespace OOSDL {
	class Window : public Bitmap, public SDLClient, private Debug {
		Uint16 fps;
		bool fullscreen;
		
		SDL_Surface* createWindow(int width, int height, bool fullscreen) throw (sdl_error);
		virtual void quit();
	public:
		Window(
				Uint16 width = OOSDL_WINDOW_DEFAULT_WIDTH,
				Uint16 height = OOSDL_WINDOW_DEFAULT_HEIGHT,
				Uint16 fps = OOSDL_WINDOW_DEFAULT_FPS,
				bool fullscreen = OOSDL_WINDOW_DEFAULT_FULLSCREEN
			)
				throw (sdl_error);
		virtual ~Window();
		
		// Métodos de controle de atualização.
		virtual Uint16 getFPS() const;
		virtual void setFPS(Uint16 fps);
		virtual void update()
				throw (std::bad_exception);
		
		// Métodos de persolanização do cabeçalho.
//		virtual std::string getTitle() const;						// Recupera o título da janela.
//		virtual void setTitle(std::string);							// Muda o título da janela.
		virtual void setIcon(const Bitmap& icon);					// Muda o ícone da janela.
		
		// GrabMode: Quando o GrabMode está ativo, os dispositivos de entrada(mouse e teclado) ficam confinados na aplicação.
		virtual bool getGrabMode() const;							// Verifica se o GrabMode está ativo no momento.
		virtual void setGrabMode(bool);								// Ativa ou desativa o GrabMode.
		
		// Métodos de controle da janela. (posicionamento e tamanho)
		virtual bool minimize() const;								// Tenta minimizar a janela, se possível. Retorna se um booleano que define se a janela foi minimizada com sucesso.
		virtual void changeResolution(Uint16 width, Uint16 height);	// Altera a resolução da janela.
		virtual bool isFullscreen() const;							// Verifica se a janela está atualmente em modo fullscreen.
		virtual void setFullscreen(bool);							// Força a janela à ligar/desligar o modo fullscreen.
		virtual bool toggleFullscreen();							// Alterna entre modo fullscreen e retorna o estado atual. (setFullscreen(!isFullscreen) e return isFullscreen)
		
		// Dispose
		virtual void dispose();										// Fecha a janela.
	};
};
#endif
