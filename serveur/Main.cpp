#include "stdafx.h"

#include "Sockethandler.h"
#include "AuthSocket.h"
#include "Config.h"

bool stopEvent = false;

int main()
{
	// Initialisation Winsock prérequis Windows
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
		system("pause");
	}
	catch (std::exception e)
	{
		std::cout << "SOCKET ERROR : " << e.what() << std::endl;
		std::cout << "WSA ERROR : " << WSAGetLastError() << std::endl;

		delete handler;
		WSACleanup();
		return 1;
	}

	delete handler;
	WSACleanup();

	return 0;
}
