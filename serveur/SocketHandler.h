#ifndef _SOCKETHANDLER_H
#define _SOCKETHANDLER_H

// G�re la socket
class SocketHandler
{
	public:
		// Repr�sente la session g�r�e par la socket
		class Session
		{
			public:
				Session(void) {};
				virtual ~Session(void) {};

				virtual void OnRead(void) = 0; // Classe abstraite
		};

		SocketHandler(void);
		virtual ~SocketHandler(void);

		void open(short);
		void set_session(Session* session);
		void wait_client(void);

		size_t recv_soft(char* buf, size_t len);
		size_t send_soft(char* buf, size_t len);

		// G�re les entr�es / sorties de la socket
		int handle_input(void);
		int handle_output(void);

	private:
		SOCKET peer;
		SOCKET slave;
		Session* session_;
};

#endif
