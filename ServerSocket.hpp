#ifndef OOSDL_SERVERSOCKET_HPP
#define OOSDL_SERVERSOCKET_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include <list>
#include "TCPSocket.hpp"
#include "ConnectionSocket.hpp"
#include "ServerListener.hpp"
#include "interrupted_while_waiting.hpp"
#include "already_connected.hpp"
#include "incorrect_format_error.hpp"

namespace OOSDL {
	class ServerSocket : public TCPSocket {
		Uint16 port;
		Thread* acceptThread;
		std::list<ServerListener*> listeners;
		
		static int listen(void*);
	public:
		ServerSocket();
		ServerSocket(Uint16 port)
				throw (net_error);
		ServerSocket(TCPsocket);
		
		// Métodos de verificação.
		virtual Uint16 getPort() const;											// Verifica em qual porta o servidor está rodando.
		virtual bool isListening();												// Verifica se a thread que aceita novas conexões está rodando.
		
		// Método de controle da thread do accept.
//		virtual void addListener(ServerListener*);								// Adiciona um listener.
//		virtual void removeListener(ServerListener*);							// Remove um listener.
//		virtual void startListening();											// Inicia a thread que irá chamar o método accept e ficar esperando por novas conexões. Quando um novo cliente se conecta, o método run de cada listener é chamado enviando a nova conexão(socket) como parâmetro.
//		virtual void stopListening();											// Encerra a thread iniciada por startListening. Se o método startListening nunca foi chamado este método não faz nada. Isto pode ser verificado com isListening().
		
		// Aceita novas conexões.
		virtual void bound(Uint16 port)
				throw (net_error, already_connected);
		virtual ConnectionSocket accept()
				throw (net_error, interrupted_while_waiting);
	};
};

#endif
