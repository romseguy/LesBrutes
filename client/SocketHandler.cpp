#include "stdafx.h"
#include "SocketHandler.h"

SocketHandler::SocketHandler(string ip) : session_(NULL), serveraddress(ip), peer(NULL)
{
}

SocketHandler::~SocketHandler()
{
	closesocket(peer);

	if (session_)
		delete session_;
}

void SocketHandler::open()
{
	// Initialisation Winsock pr�requis Windows
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	struct protoent *ppe = getprotobyname("tcp");

	if (ppe == NULL)
		fl_alert("Illegal Protocol :  %d", WSAGetLastError());

	SOCKET s = socket (AF_INET, SOCK_STREAM, ppe->p_proto);

	if (s == INVALID_SOCKET) 
		fl_alert("Socket invalide : %d", WSAGetLastError());

	struct sockaddr_in sin;
	memset (&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr (serveraddress.c_str());
	sin.sin_port = htons(5678);

	if (connect (s, (struct sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR)
		fl_alert("Echec connect : %d", WSAGetLastError());

	peer = s;
	handle_input();
}

void SocketHandler::set_session(Session* session)
{
	if (session_ != NULL)
		delete session_;

	session_ = session;
}

size_t SocketHandler::recv_soft(char* buf, size_t len)
{
	int nBytes = recv(peer, buf, len, 0);

	if (nBytes == SOCKET_ERROR)
		fl_alert("Erreur recv : %d", WSAGetLastError());

	return nBytes;
}

size_t SocketHandler::send_soft(char* buf, size_t len)
{
	int nBytes = send(peer, buf, len, 0);

	if (nBytes == 0 || nBytes == SOCKET_ERROR)
		fl_alert("Erreur send : %d", WSAGetLastError());

	return nBytes;
}

int SocketHandler::handle_input()
{
	if (session_ != NULL)
	{
		session_->OnRead();
	}

	return 0;
}
