#include "stdafx.h"
#include "SocketHandler.h"
#include "AuthSocket.h"
#include "AuthCodes.h"

// Messages
typedef struct AUTH_LOGON_CHALLENGE_C
{
	unsigned long        ip;
	char                 login[20];
	char                 pwd[20];
} sAuthLogonChallenge_C;

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

void AuthSocket::OnRead()
{
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
		{
			stringstream ss;
			ss << "Commande ";
			ss << _cmd;
			ss << " inconnue";
		}
	}
}

bool AuthSocket::HandleLogonChallenge()
{
	cout << "challenge" << endl;

	vector<unsigned char> buffer;
	buffer.resize(sizeof(sAuthLogonChallenge_C));

	handler->recv_soft((char*) buffer.data(), sizeof(sAuthLogonChallenge_C));

	sAuthLogonChallenge_C* pkt = (sAuthLogonChallenge_C*) &buffer[0];
	cout << "ip:" << ntohs(pkt->ip) << endl;
	cout << "login:" << pkt->login << endl;
	cout << "pwd:" << pkt->pwd << endl;

	return false;
}

bool AuthSocket::HandleLogonProof()
{
	cout << "logon" << endl;
	return false;
}
