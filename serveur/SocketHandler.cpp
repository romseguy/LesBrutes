#include "stdafx.h"

#include "Config.h"
#include "SocketHandler.h"

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
		throw std::runtime_error("Illegal Protocol");

	peer = socket(AF_INET, SOCK_STREAM, ppe->p_proto);

	if (peer == INVALID_SOCKET)
		throw std::runtime_error("Invalid Socket");

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(service);

	if (bind(peer, (struct sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR)
		throw std::runtime_error("Bind Error");

	if (listen(peer, 5) == SOCKET_ERROR)
		throw std::runtime_error("Listen Error");
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
		throw std::runtime_error("Invalid client socket");
}

bool SocketHandler::recv_soft(char* buf, size_t len)
{
	int bytesRcvd = 0;
	int bytesLeft = len;
	int nBytes;

	while (bytesRcvd < len)
	{
		nBytes = recv(slave, buf + bytesRcvd, bytesLeft, 0);

		if (nBytes == SOCKET_ERROR || nBytes == 0)
			return false;

		bytesRcvd += nBytes;
		bytesLeft -= nBytes;
	}

	return true;
}

bool SocketHandler::send_soft(char* buf, size_t len)
{
	int bytesSent = 0;
	int bytesLeft = len;
	int nBytes;

	while (bytesSent < len)
	{
		nBytes = send(slave, buf + bytesSent, bytesLeft, 0);

		if (nBytes == SOCKET_ERROR || nBytes == 0)
			return false;

		bytesSent += nBytes;
		bytesLeft -= nBytes;
	}

	return true;
}

void SocketHandler::handle_input()
{
	if (session_ != NULL)
		session_->OnRead();
}