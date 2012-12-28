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

// Initialisation de la structure
const AuthHandler table[] =
{
    { AUTH_LOGON_CHALLENGE,     STATUS_CONNECTED, &AuthSocket::HandleLogonChallenge    },
    { AUTH_LOGON_PROOF,         STATUS_CONNECTED, &AuthSocket::HandleLogonProof        }
};

#define AUTH_TOTAL_COMMANDS 2

AuthSocket::AuthSocket(SocketHandler* _handler) : handler(_handler)
{
}

AuthSocket::~AuthSocket()
{
}

void AuthSocket::OnRead()
{
	/*cout << "sending ..." << endl;
	const char test[5] = "test";
	int nBytes = handler->send_soft((char*) &test, 5);
	cout << "Sent : " << nBytes << " bytes" << endl;*/

	/*char test[5];
	cout << "receiving..." << endl;
	int nBytes = handler->recv_soft((char*) &test, 5);
	cout << "Recus : " << test[1] << endl;*/
	
	unsigned short _cmd;

	// on attend un short (2 octets ou 16 bits)
	if (handler->recv_soft((char *) &_cmd, 2) != 0)
	{
		_cmd = ntohs(_cmd);
		cout << "commande recue : " << _cmd << endl;

		size_t i;
		for (i = 0; i < AUTH_TOTAL_COMMANDS; ++i)
		{
			if (table[i].cmd == _cmd)
			{
				(*this.*table[i].handler)();
			}
		}

		if (i == AUTH_TOTAL_COMMANDS)
			throw exception("Commande inconnue");
	}
}

bool AuthSocket::HandleLogonChallenge()
{
	cout << "challenge" << endl;

	//if (handler->recv_len() < sizeof(sAuthLogonChallenge_C))
        return false;
}

bool AuthSocket::HandleLogonProof()
{
	cout << "logon" << endl;
	return false;
}
