#ifndef CMDSOCKET_H
#define CMDSOCKET_H

class CmdSocket : public SocketHandler::Session
{
	public:
		CmdSocket(SocketHandler* _handler) : handler(_handler), authed(false) {}
		virtual ~CmdSocket() {}

		virtual void OnRead();

		bool HandleLogon();
		bool HandleRegister();
		bool HandleInfoBrute();
		bool HandleGetImage();
		bool HandleGetOpponent();

	private:
		SocketHandler* handler;
		bool authed;
};

#endif
