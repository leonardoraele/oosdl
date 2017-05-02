#ifndef OOSDL_ConnectionSocket_HPP
#define OOSDL_ConnectionSocket_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include <string>
//#include <list>	
//#include "ConnectiontListener.hpp"
#include "TCPSocket.hpp"
#include "net_error.hpp"
#include "incorrect_format_error.hpp"
#include "connection_closed.hpp"

namespace OOSDL {
	class ConnectionSocket : public TCPSocket {
//		Thread inputListener;
//		std::list<ConnectiontListener*> listeners;
		IPaddress address;
		
		bool check_format(std::string ip, bool include_port);
	public:
		
		// Construtores.
		explicit ConnectionSocket(Uint32 host, Uint16 port)						// Cria um socket conectado ao endereço IPv4 e porta indicados.
				throw (net_error);												// Levanta exceção quando não é possível se conectar.
		explicit ConnectionSocket(std::string host, Uint16 port = 0)			// Cria um socket conectado ao endereço IPv4 indicado pela string no formato "(XXX.XXX.XXX.XXX|localhost)[:PPPP]" e porta definidos. Se a porta não for informada, é subentendido que ela está presente na string(neste caso, na representação "P" do exemplo). Localhost será entendido como endereço 127.0.0.1.
				throw (net_error, incorrect_format_error);						// Levanta exceção net_error quando não é possível se conectar ou uma incorrect_format_error quando a string não está no formato esperado.
		ConnectionSocket(TCPsocket);											// Cria um socket sob um socket SDLNet já existente.
		ConnectionSocket(ConnectionSocket);
		
		// Métodos de verificação.
//		bool isListening() const;												// Verifica se a thread de leitura do input está rodando. (se startListening já foi chamado e stopListening ainda não)
		virtual std::string getStringIP() const;
		virtual Uint32 getUint32IP() const;
		virtual Uint16 getPort() const;
		
		// Métodos de comunicação.
		virtual void send(const void*, int length)
				throw (net_error, connection_closed);
		virtual int receive(void*, int maxlen)
				throw (net_error, connection_closed);
		
		// Observer.
//		void startListening();													// Cria uma thread para ficar lendo a entrada do socket. Sempre que um novo dado for recebido, chama o método run de cada listener para tratar. Se este método for chamado mais de uma vez antes do método stopListening, as chamadas sucessivas são ignoradas. Note que se a lista de listeners estiver vazia, mensagens recebidas são ignoradas.
//		void stopListening();													// Pára a thread iniciada por startListening. Se startListening nunca foi chamado, este método não faz nada.
//		void addListener(ConnectiontListener*, bool start = false)				// Adiciona um runnable à lista dos tratadores. Sempre que dados forem recebidos, o método run deste runnable é chamado. Se start for true, chama internamente o método startListening.
		
		// Métodos SDLNet.
		virtual IPaddress get_SDLNet_IPaddress();
		virtual const IPaddress get_SDLNet_IPaddress() const;
	};
};

#endif
