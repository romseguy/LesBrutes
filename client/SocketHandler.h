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
				Session() {};
				virtual ~Session() {};

				virtual void OnRead() = 0; // Classe abstraite
		};

		SocketHandler(std::string);
		virtual ~SocketHandler();

		void open();
		void set_session(Session* session);

		size_t recv_soft(char* buf, size_t len);
		size_t send_soft(char* buf, size_t len);

		// G�re les entr�es / sorties de la socket
		int handle_input();
		int handle_output();

	private:
		std::string serveraddress;
		SOCKET peer;
		Session* session_;
};

#endif
