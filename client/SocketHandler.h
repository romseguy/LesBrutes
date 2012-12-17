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

		void open(Fl_Widget* o , void* userData);
		void set_session(Session* session);

		bool recv_soft(char* buf, size_t len);

		// G�re les entr�es / sorties de la socket
		int handle_input(void);
		int handle_output(void);

	private:
		string serveraddress;
		SOCKET peer;
		Session* session_;
};

#endif