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
				Session(void) {};
				virtual ~Session(void) {};

				virtual void OnRead(void) = 0; // Classe abstraite
		};

		SocketHandler(string);
		virtual ~SocketHandler(void);

		void open(void);
		void set_session(Session* session);

		size_t recv_soft(char* buf, size_t len);
		size_t send_soft(char* buf, size_t len);

		// Gère les entrées / sorties de la socket
		int handle_input(void);
		int handle_output(void);

	private:
		string serveraddress;
		SOCKET peer;
		Session* session_;
};

#endif
