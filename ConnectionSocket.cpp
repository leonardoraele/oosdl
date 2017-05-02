#ifndef OOSDL_ConnectionSocket_CPP
#define OOSDL_ConnectionSocket_CPP

#define OOSDL_ConnectionSocket_EXPECTED_FORMAT "(XXX.XXX.XXX.XXX|localhost)[:PPPP]" // TODO remover daqui

#include <SDL/SDL_net.h>
#include <SDL/SDL_net.h>
#include <string>
#include <cstring>
#include <sstream>
#include "ConnectionSocket.hpp"
#include "TCPSocket.cpp"
#include "net_error.cpp"
#include "connection_closed.cpp"
#include "incorrect_format_error.cpp"

bool OOSDL::ConnectionSocket::check_format(std::string ip, bool include_port)
{
	// TODO Implementar:
	// if (include_port)
	//     regex = "(XXX.XXX.XXX.XXX|localhost):PPPP"
	// else
	//     regex = "(XXX.XXX.XXX.XXX|localhost)"
	// end
	// result = (regex == ip)
	// return result
	
	return true;
}

OOSDL::ConnectionSocket::ConnectionSocket(Uint32 host, Uint16 port)
throw (net_error)
: TCPSocket()
{
	this->address.host = host;
	this->address.port = port;
	this->TCPSocket::connect(this->get_SDLNet_IPaddress());
}

OOSDL::ConnectionSocket::ConnectionSocket(std::string host, Uint16 port)
throw (net_error, incorrect_format_error)
: TCPSocket()
{
	bool correct_format = check_format(host.c_str(), port);
	if (correct_format)
	{
		if (SDLNet_ResolveHost(&address, host.c_str(), port) == 0)
		{
			throw net_error();
		}
	}
	else
	{
		throw incorrect_format_error(OOSDL_ConnectionSocket_EXPECTED_FORMAT, host.c_str());
	}
	
	this->connect(this->get_SDLNet_IPaddress());
}

OOSDL::ConnectionSocket::ConnectionSocket(TCPsocket socket)
: TCPSocket(socket), address(*SDLNet_TCP_GetPeerAddress(this->get_SDLNet_TCPsocket()))
{}

OOSDL::ConnectionSocket::ConnectionSocket(ConnectionSocket other)
: address(other.address), TCPSocket(other)
{}

std::string OOSDL::ConnectionSocket::getStringIP() const
{
	typedef Uint8 byte;
	std::stringstream builder;
	
	Uint32 host = this->getUint32IP();
	Uint16 port = this->getPort();
	byte* b = reinterpret_cast<byte*>(&host);
	
	builder << *b++;
	builder << '.';
	builder << *b++;
	builder << '.';
	builder << *b++;
	builder << '.';
	builder << *b++;
	builder << ':';
	builder << port;
	
	return builder.str();
}

Uint32 OOSDL::ConnectionSocket::getUint32IP() const
{
	return this->get_SDLNet_IPaddress().host;
}

Uint16 OOSDL::ConnectionSocket::getPort() const
{
	return this->get_SDLNet_IPaddress().port;
}

void OOSDL::ConnectionSocket::send(const void* data, int length)
throw (net_error, connection_closed)
{
	if (!this->isValid())
	{
		throw connection_closed();
	}
	
	int sent = SDLNet_TCP_Send(this->get_SDLNet_TCPsocket(), data, 10);
	if (sent < 10)
	{
		throw net_error();
	}
}

int OOSDL::ConnectionSocket::receive(void* buff, int maxlen)
throw (net_error, connection_closed)
{
	if (!this->isValid())
	{
		throw connection_closed();
	}
	
	int received = SDLNet_TCP_Recv(this->get_SDLNet_TCPsocket(), buff, maxlen);
	if (received <= 0)
	{
		throw net_error();
	}
}

IPaddress OOSDL::ConnectionSocket::get_SDLNet_IPaddress()
{
	return this->address;
}

const IPaddress OOSDL::ConnectionSocket::get_SDLNet_IPaddress() const
{
	ConnectionSocket* unconst_this = const_cast<ConnectionSocket*>(this);
	
	return unconst_this->get_SDLNet_IPaddress();
}


#endif
