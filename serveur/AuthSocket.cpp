#include "stdafx.h"
#include "SocketHandler.h"
#include "AuthSocket.h"
#include "AuthCodes.h"

enum eStatus
{
    STATUS_CONNECTED = 0,
    STATUS_AUTHED
};

// Commandes d'authentification
enum eAuthCmd
{
	AUTH_LOGON_CHALLENGE           = 0x00,
	AUTH_LOGON_PROOF               = 0x01
};

// Messages
typedef struct AUTH_LOGON_CHALLENGE_C
{
	char                 cmd;
	char                 error;
	unsigned long        ip;
} sAuthLogonChallenge_C;

// Structure de lien entre une commande et une fonction
typedef struct AuthHandler
{
	eAuthCmd             cmd;
	unsigned long        status;
	bool (AuthSocket::*handler)(void);
} AuthHandler;

const AuthHandler table[] =
{
    { AUTH_LOGON_CHALLENGE,     STATUS_CONNECTED, &AuthSocket::HandleLogonChallenge    },
    { AUTH_LOGON_PROOF,         STATUS_CONNECTED, &AuthSocket::HandleLogonProof        }
};

AuthSocket::AuthSocket(SocketHandler* _handler) : handler(_handler)
{
}

AuthSocket::~AuthSocket()
{
}

void AuthSocket::OnRead()
{
	char _cmd;

	while (true)
	{
		// on attend un char (1 octet ou 8 bits)
		if (handler->recv_soft((char *) &_cmd, 1) != 0)
			cout << "recu!";
	}
}

bool AuthSocket::HandleLogonChallenge()
{
	//if (handler->recv_len() < sizeof(sAuthLogonChallenge_C))
        return false;
}

bool AuthSocket::HandleLogonProof()
{
	return false;
}
