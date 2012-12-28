#ifndef _SOCKET_H
#define _SOCKET_H

class AuthSocket : public SocketHandler::Session
{
	public: 
		AuthSocket(SocketHandler*);
		virtual ~AuthSocket() { if (handler) delete handler; }

		virtual void OnRead(void);

		bool HandleLogonChallenge();
		bool HandleLogonProof();

	private:
		SocketHandler* handler;
};

#endif
