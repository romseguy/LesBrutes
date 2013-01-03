#include "stdafx.h"
using namespace std;
#include "SocketHandler.h"
#include "AuthWindow.h"


int main()
{
	// Initialisation Winsock prérequis Windows
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	Fl::scheme("gtk+");
	SocketHandler* handler = new SocketHandler("127.0.0.1");
	AuthWindow* login = new AuthWindow(400, 250, "Les Brutes", handler);
	handler->set_session(login);

	WSACleanup();

	return Fl::run();
}
