#include "stdafx.h"

#include "../serveur/Config.h"
#include "../serveur/SocketHandler.h"

SocketHandler::SocketHandler() : serveraddress(ip), service(port), peer(NULL), slave(NULL), session_(NULL)
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
	struct protoent *ppe = getprotobyname("tcp");

	if (ppe == NULL)
		throw std::runtime_error("Illegal Protocol");

	peer = socket(AF_INET, SOCK_STREAM, ppe->p_proto);

	if (peer == INVALID_SOCKET)
		throw std::runtime_error("Invalid Socket");

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(serveraddress.c_str());
	sin.sin_port = htons(service);

	if (connect(peer, (struct sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR)
		throw std::runtime_error("Connect Error");
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
		throw std::runtime_error("Socket Error");

	return nBytes;
}

size_t SocketHandler::send_soft(char* buf, size_t len)
{
	int nBytes = send(peer, buf, len, 0);

	if (nBytes == SOCKET_ERROR)
		throw std::runtime_error("Socket Error");

	return nBytes;
}

void SocketHandler::handle_input()
{
	if (session_ != NULL)
		session_->OnRead();
}