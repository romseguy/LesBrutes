#include "stdafx.h"

#include "../serveur/SocketHandler.h"
#include "../serveur/ByteBuffer.h"
#include "../serveur/AuthCodes.h"

#include "AuthWindow.h"

// Discriminant (commande)
enum eCmd
{
	LOGON_C                       = 0x00,
	LOGON_S                       = 0x01,
	REGISTER_C                    = 0x02
};

/** Messages
typedef struct LOGON_C
{
	byte                          cmd;
	unsigned short                size;
	char                          login;
	char                          pwd;
};

typedef struct LOGON_S
{
	byte                          cmd;
	byte                          error;
}; */

void AuthWindow::OnRead()
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

	// le serveur a accepté la connexion
	if (buf[0] == LOGON_S && buf[1] == LOGIN_OK)
		fl_alert("connecté");
}
