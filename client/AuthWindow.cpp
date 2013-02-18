#include "stdafx.h"

#include "../serveur/SocketHandler.h"
#include "../serveur/ByteBuffer.h"
#include "../serveur/AuthCodes.h"
#include "../serveur/Brute.h"

#include "MainWindow.h" // inclut aussi AuthWindow.h

bool AuthWindow::HandleLogon()
{
	ByteBuffer packet, buf;

	std::string login(txt_login->value());
	std::string pwd(txt_pwd->value());

	// requ�te LOGON_C
	packet << uint8_t(LOGON_C);
	packet << uint16_t(login.length() + pwd.length() + 2); // ne pas oublier les 0 de fin de chaine
	packet << login;
	packet << pwd;

	if (!handler->send_soft((char*) packet.contents(), packet.size())) return false;

	// r�ponse LOGON_S
	buf.resize(2);

	if (!handler->recv_soft((char*) buf.contents(), 2)) return false;
	if (buf[0] != LOGON_S) return false;

	if (buf[1] == LOGIN_KO)
	{
		fl_alert("Connexion : �chec");
		return true;
	}

	// requ�te INFO_BRUTE_C
	packet.clear();
	packet << uint8_t(INFO_BRUTE_C);
	packet << uint16_t(login.length() + 1);
	packet << login;

	if (!handler->send_soft((char*) packet.contents(), packet.size())) return false;

	// r�ponse INFO_BRUTE_S
	buf.clear();
	buf.resize(3);

	if (!handler->recv_soft((char*) buf.contents(), 3)) return false;
	if (buf[0] != INFO_BRUTE_S) return false;

	uint16_t restant;
	buf.rpos(1);
	buf >> restant;
	buf.resize(restant + buf.size());

	if (!handler->recv_soft((char*) buf.contents(3), restant)) return false;

	buf.rpos(3);
	uint8_t portraitId, level, hp, strength, speed;
	buf >> portraitId >> level >> hp >> strength >> speed;

	// requ�te GET_IMAGE_C
	packet.clear();
	packet << uint8_t(GET_IMAGE_C);
	packet << uint8_t(portraitId);

	if (!handler->send_soft((char*) packet.contents(), packet.size())) return false;

	// r�ponse GET_IMAGE_S
	buf.clear();
	buf.resize(3);

	if (!handler->recv_soft((char*) buf.contents(), 3)) return false;
	if (buf[0] != GET_IMAGE_S) return false;

	buf.rpos(1);
	buf >> restant;
	buf.resize(restant + buf.size());

	if (!handler->recv_soft((char*) buf.contents(3), restant)) return false;

	handler->set_session(new MainWindow(1024, 768, "Les Brutes", handler, new Brute(login, pwd, level, hp, strength, speed, buf.contents(3))));

	return true;
}

bool AuthWindow::HandleRegister()
{
	ByteBuffer packet, buf;

	const char* login = txt_login->value();
	const char* pwd = txt_pwd->value();

	// requ�te REGISTER_C
	packet << uint8_t(REGISTER_C);
	packet << uint16_t(strlen(login) + strlen(pwd) + 2); // ne pas oublier les 0 de fin de chaine
	packet << login;
	packet << pwd;

	if (!handler->send_soft((char*) packet.contents(), packet.size())) return false;

	// r�ponse REGISTER_S
	buf.resize(2);

	if (!handler->recv_soft((char*) buf.contents(), 2)) return false;
	if (buf[0] != REGISTER_S) return false;

	switch (buf[1])
	{
		case REGISTER_ALREADY_EXISTS:
			fl_alert("Inscription : Veuillez choisir un autre login.");
			break;
		case REGISTER_OK:
			fl_alert("Inscription : brute %s cr��e", login);
			break;
		case REGISTER_KO:
			fl_alert("Inscription : �chec");
			break;
	}

	return true;
}