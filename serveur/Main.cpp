#include "stdafx.h"

#include "SocketHandler.h"
#include "AuthSocket.h"

bool stopEvent = false;

int main()
{
	// Initialisation Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != NO_ERROR)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	try
	{
		// Ouverture socket
		SocketHandler handler;
		SOCKET master = handler.open(5678);
		handler.set_session(new AuthSocket(&handler));
		cout << "Serveur ON" << endl;

		while (!stopEvent)
		{
			cout << "Attente client" << endl;
			SOCKET slave = accept(master, NULL, 0);
			cout << "Nouveau client" << endl;

			while (slave != NULL)
			{
				handler.handle_input();
			}
		}

		closesocket(master);
	}
	catch (exception e)
	{
		printf("SOCKET ERROR : %s : %ld", e.what(), WSAGetLastError());
		WSACleanup ();
		return 1;
	}

	WSACleanup ();

	return 0;
}
