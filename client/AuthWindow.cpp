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
	unsigned short                size;
	char                          login;
	char                          pwd;
} sAuthLogonChallenge_C;

void AuthWindow::OnRead()
{	
	const char* login = txt_login->value();
	const char* pwd = txt_pwd->value();

	ByteBuffer packet;
	packet << byte(AUTH_LOGON_CHALLENGE);
	packet << unsigned short(strlen(login) + strlen(pwd) + 2); // ne pas oublier les 0 de fin de chaine

	packet << login;
	packet << pwd;

	int nBytes = handler->send_soft((char*) packet.contents(), packet.size());
	fl_alert("%d octets envoyes", nBytes);
}
