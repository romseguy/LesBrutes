#include "stdafx.h"

#include "SocketHandler.h"
#include "AuthSocket.h"
#include "AuthCodes.h"
#include "ByteBuffer.h"

// Etat de la connexion client/serveur
enum eStatus
{
	STATUS_CONNECTED = 0,
	STATUS_AUTHED
};

// Discriminant (commande)
enum eCmd
{
	LOGON_C                       = 0x00,
	LOGON_S                       = 0x01,
	REGISTER_C                    = 0x02
};

/** Messages
typedef struct LOGON_C
{
	byte                          cmd;
	unsigned short                size;
	char                          login;
	char                          pwd;
};

typedef struct LOGON_S
{
	byte                          cmd;
	byte                          error;
}; */

// Structure de lien entre une commande et une fonction
typedef struct CmdHandler
{
	eCmd                          cmd;
	byte                          status;
	bool (AuthSocket::*cmd_handler)(void);
} CmdHandler;

// Initialisation de la structure
const CmdHandler table[] =
{
	{ LOGON_C,               STATUS_CONNECTED, &AuthSocket::HandleLogon             },
	{ REGISTER_C,            STATUS_CONNECTED, &AuthSocket::HandleCreateUser        }
};

#define CLIENT_TOTAL_COMMANDS 2

void AuthSocket::OnRead()
{
	byte cmd;

	if (handler->recv_soft((char *) &cmd, 1) != 0)
	{
		size_t i;
		for (i = 0; i < CLIENT_TOTAL_COMMANDS; ++i)
		{
			if (table[i].cmd == cmd)
			{
				// On appele la fonction correspondante à la commande donnée
				if ((*this.*table[i].cmd_handler)())
					std::cout << "Commande executee" << std::endl;
				else
					std::cout << "Commande echec" << std::endl;
			}
		}

		if (i == CLIENT_TOTAL_COMMANDS)
			std::cout << "Commande inconnue" << std::endl;
	}
}

bool AuthSocket::HandleLogon()
{
	std::cout << "challenge" << std::endl;
	ByteBuffer buf, packet;

	// buffer qui va recevoir les 2 octets correspondant à la taille du paquet restant
	buf.resize(2);
	handler->recv_soft((char*) buf.contents(), 2);

	unsigned short restant;
	buf >> restant;

	// on recupère le restant des données
	buf.resize(restant + buf.size());
	handler->recv_soft((char*) buf.contents(2), restant);

	// décalage de 3 octets pour récupérer le login car rpos est remis à 0 avec le resize
	std::string l;
	buf.rpos(2);
	buf >> l;
	std::cout << l << " (login)" << std::endl;

	// mot de passe
	std::string p;
	buf >> p;
	std::cout << p << " (pwd)" << std::endl;

	// envoi de la réponse au client
	packet << byte(LOGON_S);
	packet << byte(LOGIN_OK);
	handler->send_soft((char*) packet.contents(), packet.size());

	return true;
}

bool AuthSocket::HandleCreateUser()
{
	std::cout << "register" << std::endl;

	// ("INSERT INTO user VALUES('test', '123');");
   
	return true;
}