#include "stdafx.h"
#include "SocketHandler.h"
#include "AuthSocket.h"

SocketHandler::SocketHandler() : session_(NULL)
{
}

SocketHandler::~SocketHandler()
{
}

SOCKET SocketHandler::open(short service)
{
	struct protoent *ppe = getprotobyname("tcp");

	if (ppe == NULL)
		throw exception ("Illegal Protocol");

	SOCKET s = socket (AF_INET, SOCK_STREAM, ppe->p_proto);

	if (s == INVALID_SOCKET)
		throw exception ("Invalid Socket");

	struct sockaddr_in sin;
	memset (&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons (service);

	if (bind (s, (struct sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR)
		throw exception ("Bind Error");
   
	if (listen (s, 5) == SOCKET_ERROR)
		throw exception ("Listen Error");

	peer = s;

	return s;
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
