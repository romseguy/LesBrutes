#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

class SocketHandler
{
	public:
		class Session
		{
			public:
				Session() {};
				virtual ~Session() {};

				virtual void OnRead() = 0;
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

		SOCKET peer;  // listening socket
		SOCKET slave; // client socket
		Session* session_;
};

#endif
