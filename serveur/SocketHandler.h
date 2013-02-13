#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

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

		void open();
		void set_session(Session* session);
		void wait_client();

		size_t recv_soft(char* buf, size_t len);
		size_t send_soft(char* buf, size_t len);

		void handle_input();

	private:
		std::string serveraddress;
		uint16_t service;
		SOCKET peer;  // socket serveur
		SOCKET slave; // socket client
		Session* session_;
};

#endif
