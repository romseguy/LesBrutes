#include "stdafx.h"

#include "../serveur/SocketHandler.h"
#include "../serveur/ByteBuffer.h"
#include "../serveur/AuthCodes.h"
#include "../serveur/Brute.h"

#include "MainWindow.h"

bool AuthWindow::HandleLogon()
{
	std::string login(txt_login->value());
	std::string pwd(txt_pwd->value());

	ByteBuffer packet, buf;

	// requête connexion
	packet << uint8_t(LOGON_C);
	packet << uint16_t(login.length() + pwd.length() + 2); // ne pas oublier les 0 de fin de chaine
	packet << login;
	packet << pwd;

	if (!handler->send_soft((char*) packet.contents(), packet.size())) return false;

	// réponse connexion
	buf.resize(2);

	if (!handler->recv_soft((char*) buf.contents(), 2)) return false;
	if (buf[0] != LOGON_S) return false;

	if (buf[1] == LOGIN_OK)
	{
		// requête pour récupérer les infos de notre brute
		packet.clear();
		packet << uint8_t(INFO_BRUTE_C);
		packet << uint16_t(login.length() + 1);
		packet << login;

		if (!handler->send_soft((char*) packet.contents(), packet.size())) return false;

		// réponse infos brute
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

		// requête pour récupérer l'image du portrait
		packet.clear();
		packet << uint8_t(GET_IMAGE_C);
		packet << uint8_t(portraitId);

		if (!handler->send_soft((char*) packet.contents(), packet.size())) return false;

		// réponse image portrait
		buf.clear();
		buf.resize(3);

		if (!handler->recv_soft((char*) buf.contents(), 3)) return false;
		if (buf[0] != GET_IMAGE_S) return false;

		buf.rpos(1);
		buf >> restant;
		buf.resize(restant + buf.size());

		if (!handler->recv_soft((char*) buf.contents(3), restant)) return false;

		handler->set_session(new MainWindow(1024, 768, "Les Brutes", handler, new Brute(login, pwd, level, hp, strength, speed, buf.contents(3))));
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

	if (!handler->send_soft((char*) packet.contents(), packet.size())) return false;

	// réponse
	buf.resize(2);

	if (!handler->recv_soft((char*) buf.contents(), 2)) return false;
	if (buf[0] != REGISTER_S) return false;

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