#include "stdafx.h"
#include "SocketHandler.h"
#include "AuthSocket.h"
#include "AuthCodes.h"

// Etat du client
enum eStatus
{
	STATUS_CONNECTED = 0,
	STATUS_AUTHED
};

// Commandes
enum eAuthCmd
{
	AUTH_LOGON_CHALLENGE           = 0x00,
	AUTH_LOGON_PROOF               = 0x01
};

// Messages
typedef struct AUTH_LOGON_CHALLENGE_C
{
	unsigned char                 cmd;
	unsigned char                 login[20];
	unsigned char                 pwd[20];
} sAuthLogonChallenge_C;

// Structure de lien entre une commande et une fonction
typedef struct AuthHandler
{
	eAuthCmd             cmd;
	unsigned long        status;
	bool (AuthSocket::*cmd_handler)(void);
} AuthHandler;

// Initialisation de la structure
const AuthHandler table[] =
{
	{ AUTH_LOGON_CHALLENGE,     STATUS_CONNECTED, &AuthSocket::HandleLogonChallenge    },
	{ AUTH_LOGON_PROOF,         STATUS_CONNECTED, &AuthSocket::HandleLogonProof        }
};

#define AUTH_TOTAL_COMMANDS 2

void AuthSocket::OnRead()
{
	unsigned char cmd;

	// On attend un char (1 octet ou 8 bits)
	if (handler->recv_soft((char *) &cmd, 1) != 0)
	{
		size_t i;
		for (i = 0; i < AUTH_TOTAL_COMMANDS; ++i)
		{
			if (table[i].cmd == cmd)
			{
				// On appele la fonction correspondante à la commande donnée
				(*this.*table[i].cmd_handler)();
			}
		}

		if (i == AUTH_TOTAL_COMMANDS)
		{
			stringstream ss;
			ss << "Commande inconnue" << endl;
		}
	}
}

bool AuthSocket::HandleLogonChallenge()
{
	cout << "challenge" << endl;

	// On crée le buffer qui va recevoir les octets associés à la commande
	vector<unsigned char> buffer;
	buffer.resize(sizeof(sAuthLogonChallenge_C));

	handler->recv_soft((char*) buffer.data(), sizeof(sAuthLogonChallenge_C));

	// On se sert de la structure définit plus haut pour cast et lire le buffer
	sAuthLogonChallenge_C* pkt = (sAuthLogonChallenge_C*) &buffer[0];
	cout << "login:" << pkt->login << endl;
	cout << "pwd:" << pkt->pwd << endl;

	return true;
}

bool AuthSocket::HandleLogonProof()
{
	cout << "logon" << endl;
	return false;
}
