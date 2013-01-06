#include "stdafx.h"

using namespace std;
#include "SocketHandler.h"
#include "AuthWindow.h"
#include "ByteBuffer.h"

// Commandes
enum eAuthCmd
{
	AUTH_LOGON_CHALLENGE           = 0x00,
	AUTH_LOGON_PROOF               = 0x01
};

// Messages
typedef struct AUTH_LOGON_CHALLENGE_C
{
	byte                          cmd;
	byte                          error;
	unsigned short                size;
	char                          login[20];
	char                          pwd[20];
} sAuthLogonChallenge_C;

void AuthWindow::OnRead()
{
	// commande serveur
	byte cmd = AUTH_LOGON_CHALLENGE;
	handler->send_soft((char*) &cmd, 1);
	
	ByteBuffer packet;
	// en tete
	packet << cmd;
	packet << byte(0);
	packet << unsigned short(sizeof(txt_login->value())+sizeof(txt_pwd->value()));

	// login
	string l(txt_login->value());
	//l.resize(19, ' '); // \0 est rajouté
	packet << l;

	// mot de passe
	string p(txt_pwd->value());
	//p.resize(19, ' ');
	packet << p;

	int nBytes = handler->send_soft((char*) packet.contents(), packet.size());
	fl_alert("%d octets envoyes", nBytes);
}
