#include "stdafx.h"

#include "../serveur/SocketHandler.h"
#include "../serveur/ByteBuffer.h"
#include "../serveur/AuthCodes.h"

#include "AuthWindow.h"

/** Messages
struct LOGON_C, REGISTER_C
{
	byte                          cmd;
	unsigned short                size;
	char                          login;
	char                          pwd;
};

struct LOGON_S, REGISTER_S
{
	byte                          cmd;
	byte                          error;
};
**/

void AuthWindow::OnRead()
{

}

bool AuthWindow::HandleLogon()
{
	const char* login = txt_login->value();
	const char* pwd = txt_pwd->value();
	ByteBuffer packet, buf;

	// requête
	packet << byte(LOGON_C);
	packet << unsigned short(strlen(login) + strlen(pwd) + 2); // ne pas oublier les 0 de fin de chaine
	packet << login;
	packet << pwd;

	handler->send_soft((char*) packet.contents(), packet.size());

	// réponse
	buf.resize(2);
	handler->recv_soft((char*) buf.contents(), 2);

	return buf[0] == LOGON_S && buf[1] == LOGIN_OK;
}

bool AuthWindow::HandleRegister()
{
	const char* login = txt_login->value();
	const char* pwd = txt_pwd->value();
	ByteBuffer packet, buf;

	// requête
	packet << byte(REGISTER_C);
	packet << unsigned short(strlen(login) + strlen(pwd) + 2); // ne pas oublier les 0 de fin de chaine
	packet << login;
	packet << pwd;

	handler->send_soft((char*) packet.contents(), packet.size());

	// réponse
	buf.resize(2);
	handler->recv_soft((char*) buf.contents(), 2);

	return buf[0] == REGISTER_S && buf[1] == REGISTER_OK;
}
