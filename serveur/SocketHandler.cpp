#include "stdafx.h"
#include "SocketHandler.h"
#include "AuthSocket.h"

SocketHandler::SocketHandler() : session_(NULL), peer(NULL), slave(NULL)
{
}

SocketHandler::~SocketHandler()
{
	closesocket(peer);
	closesocket(slave);

	if (session_)
		delete session_;
}

void SocketHandler::open(short service)
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
}

void SocketHandler::set_session(Session* session)
{
	if (session_ != NULL)
		delete session_;

	session_ = session;
}

void SocketHandler::wait_client()
{
	slave = accept(peer, NULL, 0);

	if (slave == INVALID_SOCKET)
		throw exception("Socket client invalide");
}

size_t SocketHandler::recv_soft(char* buf, size_t len)
{
	int nBytes = recv(slave, buf, len, 0);

	if (nBytes == SOCKET_ERROR)
		throw exception("recv");

	return nBytes;
}

size_t SocketHandler::send_soft(char* buf, size_t len)
{
	int nBytes = send(slave, buf, len, 0);

	if (nBytes == 0 || nBytes == SOCKET_ERROR)
		throw exception("send");

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
