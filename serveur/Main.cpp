#include "stdafx.h"
#include "Sockethandler.h"
#include "AuthSocket.h"

bool stopEvent = false;

int main()
{
	// Initialisation Winsock prérequis Windows
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	try
	{
		// Ouverture socket d'écoute du réseau sur le port 5678
		SocketHandler* handler = new SocketHandler();
		handler->open(5678);
		handler->set_session(new AuthSocket(handler));
		cout << "Serveur ON" << endl;

		while (!stopEvent)
		{
			cout << "Attente client" << endl;
			handler->wait_client();
			cout << "Nouveau client" << endl;

			bool end = false;
			while (!end)
			{
				handler->handle_input();
			}
		}

		delete handler;
	}
	catch (exception e)
	{
		cout << "SOCKET ERROR : " << e.what() << endl;
		cout << "WSA ERROR : " << WSAGetLastError() << endl;
		system("pause");
		WSACleanup();
		return 1;
	}

	WSACleanup ();

	return 0;
}
