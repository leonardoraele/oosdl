#ifndef OOSDL_TCPSocket_HPP
#define OOSDL_TCPSocket_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include "NetClient.hpp"
#include "net_error.hpp"
#include "already_connected.hpp"

namespace OOSDL {
	class TCPSocket : public NetClient {
		TCPsocket sock;
		bool connected;
	protected:
		virtual void connect(IPaddress)
				throw (net_error, already_connected);
		virtual void close();
		virtual void set_SDLNet_TCPsocket(TCPsocket);
	public:
		TCPSocket();
		TCPSocket(const IPaddress&)
				throw (net_error);
		TCPSocket(TCPsocket sock);
		~TCPSocket();
		
		virtual bool isValid() const;
		
		virtual TCPsocket get_SDLNet_TCPsocket();
		virtual const TCPsocket get_SDLNet_TCPsocket() const;
	};
};

#endif
