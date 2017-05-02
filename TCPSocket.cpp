#ifndef OOSDL_TCPSOCKET_CPP
#define OOSDL_TCPSOCKET_CPP

#include <SDL/SDL_net.h>
#include "TCPSocket.hpp"
#include "NetClient.cpp"
#include "net_error.cpp"
#include "already_connected.cpp"

OOSDL::TCPSocket::TCPSocket()
: connected(false), NetClient(NULL)
{}

OOSDL::TCPSocket::TCPSocket(const IPaddress& address)
throw (net_error)
: connected(false), NetClient(NULL)
{
	this->connect(address);
}

OOSDL::TCPSocket::TCPSocket(TCPsocket sock)
: sock(sock), connected(true), NetClient(NULL)
{}

OOSDL::TCPSocket::TCPSocket(TCPSocket other)
: sock(other.sock), connected(other.connected), NetClient(NULL)
{}

OOSDL::TCPSocket::~TCPSocket()
{
	if (this->isValid())
	{
		this->close();
	}
}

void OOSDL::TCPSocket::connect(IPaddress address)
throw (net_error, already_connected)
{
	// Verifica se já não está conectado.
	if (this->isValid())
	{
		throw already_connected();
	}
	// Se não estiver conectado, realiza uma nova conexão.
	else
	{
		// Cria a conexão.
		TCPsocket socket = SDLNet_TCP_Open(&address);
		
		// Valida a conexão criada.
		if (socket)
		{
			this->set_SDLNet_TCPsocket(socket);
			this->connected = true;
		}
		else
		{
			throw net_error();
		}
	}
}

void OOSDL::TCPSocket::set_SDLNet_TCPsocket(TCPsocket sock)
{
	this->sock = sock;
}

void OOSDL::TCPSocket::close()
{
	if (this->isValid())
	{
		SDLNet_TCP_Close(this->sock);
		this->connected = false;
	}
}

bool OOSDL::TCPSocket::isValid() const
{
	return this->connected;
}

TCPsocket OOSDL::TCPSocket::get_SDLNet_TCPsocket()
{
	return this->sock;
}

const TCPsocket OOSDL::TCPSocket::get_SDLNet_TCPsocket() const
{
	TCPSocket* unconst_this = const_cast<TCPSocket*>(this);
	
	return unconst_this->get_SDLNet_TCPsocket();
}

#endif
