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

		SOCKET open(short);
		void set_session(Session* session);

		// G�re les entr�es / sorties de la socket
		int handle_input(void);
		int handle_output(void);

	private:
		Session* session_;
};

#endif