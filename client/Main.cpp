#include "stdafx.h"
#include "SocketHandler.h"
#include "AuthWindow.h"

int main()
{
	// Initialisation Winsock prérequis Windows
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != NO_ERROR)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	Fl::scheme("gtk+");
	SocketHandler handler;
	handler.set_session(new AuthWindow(400, 250, "Les Brutes", &handler));

	WSACleanup();

	return Fl::run();
}
