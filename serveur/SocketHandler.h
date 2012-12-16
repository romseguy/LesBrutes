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

		SocketHandler(void);
		virtual ~SocketHandler(void);

		SOCKET open(short);
		void set_session(Session* session);

		// Gère les entrées / sorties de la socket
		int handle_input(void);
		int handle_output(void);

	private:
		Session* session_;
};

#endif