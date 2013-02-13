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
	char[]                        login;
	char[]                        pwd;
};

struct LOGON_S, REGISTER_S
{
	uint8_t                       cmd;
	uint8_t                       error;
};

struct INFO_BRUTE_C
{
	uint8_t                       cmd;
	uint16_t                      size;
	char[]                        login;
};

struct INFO_BRUTE_S
{
	uint8_t                       cmd;
	uint16_t                      size;
	uint8_t                       level;
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

	// requête connexion
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

	if (buf[1] == LOGIN_OK)
	{
		// requête pour récupérer les infos de notre brute
		packet.clear();
		packet << uint8_t(INFO_BRUTE_C);
		packet << uint16_t(strlen(login) + 1);
		packet << login;

		if (!handler->send_soft((char*) packet.contents(), packet.size()))
			return false;

		// réponse
		buf.clear();
		buf.resize(3);

		if (!handler->recv_soft((char*) buf.contents(), 3))
			return false;

		if (buf[0] != INFO_BRUTE_S)
			return false;

		uint16_t restant;
		buf.rpos(1);
		buf >> restant;

		// on recupère le restant des données
		buf.resize(restant + buf.size());

		if (!handler->recv_soft((char*) buf.contents(3), restant))
			return false;

		buf.rpos(3);
		uint8_t level;
		buf >> level;
	}
	else
		fl_alert("Connexion : échec");

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