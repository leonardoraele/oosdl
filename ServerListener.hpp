#ifndef OOSDL_ServerListener_HPP
#define OOSDL_ServerListener_HPP

namespace OOSDL {
	class ServerListener {
	public:
		virtual bool onNewConnection(ConnectionSocket newConnection) = 0;
	};
};

#endif
