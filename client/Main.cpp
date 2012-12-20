#include "stdafx.h"
#include "SocketHandler.h"
#include "AuthWindow.h"

int main()
{
	// Initialisation Winsock prérequis Windows
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	Fl::scheme("gtk+");
	SocketHandler* handler = new SocketHandler("127.0.0.1");
	handler->set_session(new AuthWindow(400, 250, "Les Brutes", handler));

	WSACleanup();

	return Fl::run();
}
