#include "stdafx.h"

#include "../serveur/SocketHandler.h"

#include "AuthWindow.h"

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	Fl::scheme("gtk+");

	SocketHandler* handler = new SocketHandler();

	try
	{
		handler->open();
		handler->set_session(new AuthWindow(400, 250, "Les Brutes", handler));
	}
	catch (std::runtime_error& e)
	{
		fl_alert("SOCKET ERROR : %s", e.what());
		fl_alert("WSA ERROR : %d", WSAGetLastError());

		delete handler;
		WSACleanup();
		return 1;
	}

	return Fl::run();
}