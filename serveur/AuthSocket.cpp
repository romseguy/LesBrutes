#include "stdafx.h"
using namespace std;
#include "SocketHandler.h"
#include "AuthSocket.h"
#include "AuthCodes.h"
#include "ByteBuffer.h"

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
	byte                          cmd;
	byte                          error;
	unsigned short                size;
	char                          login[20];
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
	byte cmd;

	if (handler->recv_soft((char *) &cmd, 1) != 0)
	{
		size_t i;
		for (i = 0; i < AUTH_TOTAL_COMMANDS; ++i)
		{
			if (table[i].cmd == cmd)
			{
				// On appele la fonction correspondante � la commande donn�e
				if ((*this.*table[i].cmd_handler)())
					cout << "Commande executee" << endl;
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

	// On cr�e le buffer qui va recevoir les 4 premier octets
	ByteBuffer buf;
	buf.resize(4);
	handler->recv_soft((char*) buf.contents(), 4);

	// d�calage de 2 octets pour recuperer le nombre d'octets restant
	unsigned short restant;
	buf.rpos(2);
	buf >> restant;
	cout << restant << " octets restant" << endl;

	if (restant < sizeof(sAuthLogonChallenge_C) - buf.size())
		return false;

	// rpos est remis � 0 avec le resize
	buf.resize(restant + buf.size());
	int nBytes = handler->recv_soft((char*) buf.contents(4), restant);
	cout << nBytes << " octets recus" << endl;

	// d�calage de 4 octets pour r�cup�rer le login
	string l;
	buf.rpos(4);
	buf >> l;
	cout << l << " (login)" << endl;

	return true;
}

bool AuthSocket::HandleLogonProof()
{
	cout << "logon" << endl;
	return false;
}
