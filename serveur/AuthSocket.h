#ifndef _SOCKET_H
#define _SOCKET_H

class AuthSocket : public SocketHandler::Session
{
	public:
		AuthSocket(SocketHandler* _handler) : handler(_handler) {};
		virtual ~AuthSocket() { if (handler) delete handler; }

		virtual void OnRead(void);

		bool HandleLogonChallenge();
		bool HandleLogonProof();
		bool HandleCreateUser();

	private:
		SocketHandler* handler;
};

#endif
