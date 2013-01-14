#include "stdafx.h"

#include "../serveur/SocketHandler.h"

#include "AuthWindow.h"

int main()
{
	// Initialisation Winsock prérequis Windows
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	Fl::scheme("gtk+");
	SocketHandler* handler = new SocketHandler();
	handler->open();
	AuthWindow* login = new AuthWindow(400, 250, "Les Brutes", handler);
	handler->set_session(login);

	return Fl::run();
}
