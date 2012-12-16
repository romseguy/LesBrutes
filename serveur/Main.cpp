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
		wprintf(L"WSAStartup failed with error: %ld\n", iResult);
		return 1;
	}

	// Ouverture socket
	SocketHandler handler;
	SOCKET master = handler.open(5678);
	handler.set_session(new AuthSocket(&handler));

	while (!stopEvent)
	{
		SOCKET slave = accept(master, NULL, 0);
		cout << "Nouveau client" << endl;

		while (slave != NULL)
		{
			handler.handle_input();
		}
	}

	return 0;
}
