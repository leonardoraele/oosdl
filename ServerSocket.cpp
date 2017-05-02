#ifndef OOSDL_SERVERSOCKET_CPP
#define OOSDL_SERVERSOCKET_CPP

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include "ServerSocket.hpp"
#include "TCPSocket.cpp"
#include "ConnectionSocket.cpp"
#include "net_error.cpp"
#include "interrupted_while_waiting.cpp"
#include "already_connected.cpp"
#include "incorrect_format_error.cpp"

OOSDL::ServerSocket::ServerSocket()
: TCPSocket(), acceptThread(NULL)
{}

OOSDL::ServerSocket::ServerSocket(Uint16 port)
throw (net_error)
: TCPSocket(), acceptThread(NULL)
{
	this->bound(port);
}

OOSDL::ServerSocket::ServerSocket(TCPsocket sock)
: TCPSocket(sock), acceptThread(NULL)
{}

int listen(void* server)
{
	ServerSocket* server = (ServerSocket*) server;
	while (server->isListening())
	{
		server->accept();
	}
}

Uint16 OOSDL::ServerSocket::getPort() const
{
	return this->isValid() ? this->port : 0;
}

bool isListening()
{
	return acceptThread != NULL && acceptThread->isRunning();
}

void OOSDL::ServerSocket::bound(Uint16 port)
throw (net_error, already_connected)
{
	IPaddress address;
	SDLNet_ResolveHost(&address, NULL, port);
	
	this->connect(address);
}

OOSDL::ConnectionSocket OOSDL::ServerSocket::accept()
throw (net_error, interrupted_while_waiting) // TODO Verificar por interrupções.
{
	TCPsocket sock = SDLNet_TCP_Accept(this->get_SDLNet_TCPsocket());
	
	if (sock == NULL)
	{
		throw net_error();
	}
	
	ConnectionSocket newConnection(sock);
	
	for (auto iter = listeners.begin(); iter != iterators.end(); iter++)
	{
		if (iter != NULL)
		{
			(*iter).onNewConnection(newConnection);
		}
	}
	
	return ConnectionSocket(newConnection);
}

#endif
