#include "stdafx.h"
#include "SocketHandler.h"
#include "AuthWindow.h"

// Messages
typedef struct AUTH_LOGON_CHALLENGE
{
	unsigned long        ip;
	char                 login[20];
	char                 pwd[20];
} sAuthLogonChallenge;

void AuthWindow::OnRead()
{
	unsigned short cmd_ = htons(0);
	handler->send_soft((char*) &cmd_, 2);

	struct AUTH_LOGON_CHALLENGE login_packet;
	memset (&login_packet, 0, sizeof(login_packet));
	login_packet.ip = htons(127);
	strcpy(login_packet.login, "test");
	strcpy(login_packet.pwd, "123");

	handler->send_soft((char*) &login_packet, sizeof(sAuthLogonChallenge));
}
