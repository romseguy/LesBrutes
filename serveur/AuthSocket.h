#ifndef _SOCKET_H
#define _SOCKET_H

class AuthSocket : public SocketHandler::Session
{
	public:
		AuthSocket(SocketHandler* _handler) : handler(_handler) { authed = false; };
		virtual ~AuthSocket() { if (handler) delete handler; }

		virtual void OnRead();

		bool HandleLogon();
		bool HandleCreateUser();

	private:
		SocketHandler* handler;
		bool authed;
};

#endif
