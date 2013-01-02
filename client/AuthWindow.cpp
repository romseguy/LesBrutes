#include "stdafx.h"
#include "SocketHandler.h"
#include "AuthWindow.h"

// Commandes
enum eAuthCmd
{
	AUTH_LOGON_CHALLENGE           = 0x00,
	AUTH_LOGON_PROOF               = 0x01
};

// Messages
typedef struct AUTH_LOGON_CHALLENGE_C
{
	unsigned char        cmd;
	unsigned char        login[20];
	unsigned char        pwd[20];
} sAuthLogonChallenge_C;

void AuthWindow::OnRead()
{
	unsigned char cmd = AUTH_LOGON_CHALLENGE;
	handler->send_soft((char*) &cmd, 1);

	sAuthLogonChallenge_C login_packet;
	login_packet.cmd = AUTH_LOGON_CHALLENGE;
	memcpy(login_packet.login, "test", 20);
	memcpy(login_packet.pwd, "123", 20);

	handler->send_soft((char*) &login_packet, sizeof(login_packet));
}
