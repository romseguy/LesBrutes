#ifndef _SOCKETHANDLER_H
#define _SOCKETHANDLER_H

// Gère la socket
class SocketHandler
{
	public:
		// Représente la session gérée par la socket
		class Session
		{
			public:
				Session() {};
				virtual ~Session() {};

				virtual void OnRead() = 0; // Classe abstraite
		};

		SocketHandler();
		virtual ~SocketHandler();

		void open(unsigned short);
		void set_session(Session* session);
		void wait_client();

		size_t recv_soft(char* buf, size_t len);
		size_t send_soft(char* buf, size_t len);

		int handle_input();

	private:
		SOCKET peer;
		SOCKET slave;
		Session* session_;
};

#endif
