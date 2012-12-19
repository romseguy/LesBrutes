#include "stdafx.h"
#include "SocketHandler.h"

SocketHandler::SocketHandler() : session_(NULL), serveraddress("127.0.0.1")
{
}

SocketHandler::~SocketHandler()
{
}

void SocketHandler::open()
{
	SOCKET s = socket (AF_INET, SOCK_STREAM, 0);

	if (s == INVALID_SOCKET) 
		fl_alert("Socket invalide. Exception no : %ld", WSAGetLastError());

	struct sockaddr_in sin;
	memset (&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr (serveraddress.c_str());
	sin.sin_port = htons(5678);

	if (connect (s, (struct sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR)
		fl_alert("Echec connect. Exception no : %ld", WSAGetLastError());

	peer = s;
}

void SocketHandler::set_session(Session* session)
{
	if (session_ != NULL)
		delete session_;

	session_ = session;
}

bool SocketHandler::recv_soft(char* buf, size_t len)
{
	if (recv(peer, buf, len, NULL) < 0)
		throw exception("Erreur : %d", WSAGetLastError());

	return true;
}

int SocketHandler::handle_input()
{
	if (session_ != NULL)
    {
        session_->OnRead();
    }

	return 0;
}
