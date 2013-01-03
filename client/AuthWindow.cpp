#include "stdafx.h"
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
	byte        cmd;
	byte        login[20];
	byte        pwd[20];
} sAuthLogonChallenge_C;

void AuthWindow::OnRead()
{
	byte cmd = AUTH_LOGON_CHALLENGE;
	handler->send_soft((char*) &cmd, 1);
	
	ByteBuffer packet;
	packet << cmd;
/*	packet << string("test");
	packet << string("123");*

	handler->send_soft((char*) packet.contents(), packet.size());*/
}
