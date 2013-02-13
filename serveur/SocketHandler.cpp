#include "stdafx.h"

#include "Config.h"
#include "SocketHandler.h"
#include "AuthSocket.h"

SocketHandler::SocketHandler() : serveraddress(ip), service(port), peer(NULL), slave(NULL), session_(NULL)
{
}

SocketHandler::~SocketHandler()
{
	if (session_)
		delete session_;

	closesocket(slave);
	closesocket(peer);
}

void SocketHandler::open()
{
	struct protoent *ppe = getprotobyname("tcp");

	if (ppe == NULL)
		throw std::exception ("Illegal Protocol");

	SOCKET s = socket (AF_INET, SOCK_STREAM, ppe->p_proto);

	if (s == INVALID_SOCKET)
		throw std::exception ("Invalid Socket");

	struct sockaddr_in sin;
	memset (&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons (service);

	if (bind (s, (struct sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR)
		throw std::exception ("Bind Error");

	if (listen (s, 5) == SOCKET_ERROR)
		throw std::exception ("Listen Error");

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
		throw std::exception("Socket client invalide");
}

size_t SocketHandler::recv_soft(char* buf, size_t len)
{
	int nBytes = recv(slave, buf, len, 0);

	if (nBytes == SOCKET_ERROR)
		throw std::exception("recv");

	return nBytes;
}

size_t SocketHandler::send_soft(char* buf, size_t len)
{
	int nBytes = send(slave, buf, len, 0);

	if (nBytes == 0 || nBytes == SOCKET_ERROR)
		throw std::exception("send");

	return nBytes;
}

void SocketHandler::handle_input()
{
	if (session_ != NULL)
	{
		session_->OnRead();
	}
}