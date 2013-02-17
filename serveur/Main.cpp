#include "stdafx.h"

#include "Sockethandler.h"
#include "AuthSocket.h"
#include "Config.h"

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SocketHandler* handler = new SocketHandler();

	try
	{
		handler->open();
		handler->set_session(new AuthSocket(handler));
		std::cout << "Serveur ON" << std::endl;
		handler->wait_client();
		std::cout << "Nouveau client" << std::endl;
		handler->handle_input();
	}
	catch (std::runtime_error& e)
	{
		std::cout << "EXCEPTION : " << e.what() << std::endl;
		std::cout << "WSA ERROR : " << WSAGetLastError() << std::endl;

		delete handler;
		WSACleanup();
		return 1;
	}

	delete handler;
	WSACleanup();

	return 0;
}