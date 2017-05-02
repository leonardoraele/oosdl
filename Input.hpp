#ifndef OOSDL_Input_HPP
#define OOSDL_Input_HPP

#include <SDL/SDL.h>
#include <stdexcept>
#include <map>
#include <vector>

namespace OOSDL {
	class Input {
		std::map<int, int> keys;											// Tabela que relaciona cada tecla à seu estado atual.	<SDL Key, KeyState>
		
		void set(int, int);													// Altera o estado de uma tecla.
	protected:
		static std::vector<SDL_Event> searchEvents(Uint32 mask);		// Procura e remove da pilha todos os eventos assossiados à máscara e retorna em um vetor.
	public:
		virtual ~Input(){};
		
		// Dedine os possíveis estados de cada tecla.
		enum KeyState { KeyState = 0
			, RELEASED		// Não está sendo pressionada.
			, TRIGGERED		// Acabou de ser pressionada.
			, PUSHED		// Está sendo pressionada há, pelo menos, uma atualização.
		,KeyState_End};
		
		// Verificam os estados das teclas:
		virtual bool isDown(int) const;										// Verifica se uma tecla está sendo pressionada.
		virtual bool trigger(int) const;									// Verifica se uma tecla acabou de ser pressionada.
		virtual bool press(int) const;										// Verifica se uma tecla já estava sendo pressionada há mais de uma atualização.
		virtual bool trigger() const;										// Verifica se há ao menos uma tecla sendo pressionada no momento.
		
		// Altera o estado das teclas.
		virtual void reset();												// Retorna todas as teclas ao estado original.
		virtual int get(int) const;											// Recupera o estado atual de uma determinada tecla.
		virtual void push(int);												// Força uma tecla à ser pressionada.
		virtual void release(int);											// Força uma tecla à deixar de ser pressionada.
		
		// Atualiza as entradas para seus novos estados.
		virtual void update() throw (std::bad_exception) = 0;
	};
};
#endif
