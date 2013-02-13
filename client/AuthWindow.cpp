#include "stdafx.h"

#include "../serveur/SocketHandler.h"
#include "../serveur/ByteBuffer.h"
#include "../serveur/AuthCodes.h"

#include "AuthWindow.h"

/** Messages
struct LOGON_C, REGISTER_C
{
	uint8_t                       cmd;
	uint16_t                      size;
	char                          login;
	char                          pwd;
};

struct LOGON_S, REGISTER_S
{
	uint8_t                          cmd;
	uint8_t                          error;
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
	packet << uint8_t(LOGON_C);
	packet << uint16_t(strlen(login) + strlen(pwd) + 2); // ne pas oublier les 0 de fin de chaine
	packet << login;
	packet << pwd;

	if (!handler->send_soft((char*) packet.contents(), packet.size()))
		return false;

	// réponse
	buf.resize(2);

	if (!handler->recv_soft((char*) buf.contents(), 2))
		return false;

	if (buf[0] != LOGON_S)
		return false;

	switch (buf[1])
	{
		case LOGIN_OK:
			resize(50, 50, 1024, 768);
			delete form_login;
			redraw();
			break;
		case LOGIN_KO:
			fl_alert("Connexion : échec");
			break;
	}

	return true;
}

bool AuthWindow::HandleRegister()
{
	const char* login = txt_login->value();
	const char* pwd = txt_pwd->value();
	ByteBuffer packet, buf;

	// requête
	packet << uint8_t(REGISTER_C);
	packet << uint16_t(strlen(login) + strlen(pwd) + 2); // ne pas oublier les 0 de fin de chaine
	packet << login;
	packet << pwd;

	if (!handler->send_soft((char*) packet.contents(), packet.size()))
		return false;

	// réponse
	buf.resize(2);

	if (!handler->recv_soft((char*) buf.contents(), 2))
		return false;

	if (buf[0] != REGISTER_S)
		return false;

	switch (buf[1])
	{
		case REGISTER_ALREADY_EXISTS:
			fl_alert("Inscription : Veuillez choisir un autre login.");
			break;
		case REGISTER_OK:
			fl_alert("Inscription : brute %s créée", login);
			break;
		case REGISTER_KO:
			fl_alert("Inscription : échec");
			break;
	}

	return true;
}