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
} sAuthLogonChallenge_C;

void AuthWindow::OnRead()
{
	byte cmd = AUTH_LOGON_CHALLENGE;
	handler->send_soft((char*) &cmd, 1);
	
	ByteBuffer packet;
	packet << cmd;
	packet << byte(0);
	packet << unsigned short(21);

	string l = "test";
	l.resize(20);
	packet << l;

	handler->send_soft((char*) packet.contents(), packet.size());
}
