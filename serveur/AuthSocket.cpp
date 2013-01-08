#include "stdafx.h"

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

// Discriminant
enum eAuthCmd
{
	AUTH_LOGON_CHALLENGE           = 0x00,
	AUTH_LOGON_PROOF               = 0x01,
	AUTH_REGISTER                  = 0x02
};

// Messages
typedef struct AUTH_LOGON_CHALLENGE_C
{
	byte                          cmd;
	unsigned short                size;
	char                          login;
	char                          pwd;
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
	{ AUTH_LOGON_PROOF,         STATUS_CONNECTED, &AuthSocket::HandleLogonProof        },
	{ AUTH_REGISTER,            STATUS_CONNECTED, &AuthSocket::HandleCreateUser        }
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
					std::cout << "Commande executee" << std::endl;
				else
					std::cout << "Commande echec" << std::endl;
			}
		}

		if (i == AUTH_TOTAL_COMMANDS)
			std::cout << "Commande inconnue" << std::endl;
	}
}

bool AuthSocket::HandleLogonChallenge()
{
	std::cout << "challenge" << std::endl;
	ByteBuffer buf;

	// buffer qui va recevoir les 2 octets correspondant � la taille du paquet restant
	buf.resize(2);
	handler->recv_soft((char*) buf.contents(), 2);

	unsigned short restant;
	buf >> restant;

	// on recup�re le restant des donn�es
	buf.resize(restant + buf.size());
	handler->recv_soft((char*) buf.contents(2), restant);

	// d�calage de 3 octets pour r�cup�rer le login car rpos est remis � 0 avec le resize
	std::string l;
	buf.rpos(2);
	buf >> l;
	std::cout << l << " (login)" << std::endl;

	// mot de passe
	std::string p;
	buf >> p;
	std::cout << p << " (pwd)" << std::endl;

	return true;
}

bool AuthSocket::HandleLogonProof()
{
	std::cout << "logon" << std::endl;
	return false;
}

bool AuthSocket::HandleCreateUser()
{
	std::cout << "register" << std::endl;

	// ("INSERT INTO user VALUES('test', '123');");
   
	return true;
}