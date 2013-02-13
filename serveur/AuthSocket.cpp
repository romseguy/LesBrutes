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

/** Messages
struct LOGON_C, REGISTER_C
{
	uint8_t                       cmd;
	uint16_t                      size;
	char                          login;
	char                          pwd;
};

struct LOGON_S, REGISTER_S
{
	uint8_t                       cmd;
	uint8_t                          error;
};
**/

// Structure de lien entre une commande et une fonction
typedef struct CmdHandler
{
	eCmd                          cmd;
	uint8_t                       status;
	bool (AuthSocket::*cmd_handler)(void);
} CmdHandler;

// Initialisation de la structure
const CmdHandler table[] =
{
	{ LOGON_C,               STATUS_CONNECTED, &AuthSocket::HandleLogon             },
	{ REGISTER_C,            STATUS_CONNECTED, &AuthSocket::HandleRegister          }
};

#define CLIENT_TOTAL_COMMANDS 2

void AuthSocket::OnRead()
{
	uint8_t cmd;

	while (true)
	{
		std::cout << "Attente d'une commande du client" << std::endl;

		if (handler->recv_soft((char *) &cmd, 1))
		{
			size_t i;
			for (i = 0; i < CLIENT_TOTAL_COMMANDS; ++i)
			{
				if (table[i].cmd == cmd && (table[i].status == STATUS_CONNECTED || (authed && table[i].status == STATUS_AUTHED)))
				{
					// On appele la fonction correspondante à la commande donnée
					if ((*this.*table[i].cmd_handler)())
						std::cout << "Commande : executee" << std::endl;
					else
					{
						std::cout << "Commande : echec" << std::endl;
						return;
					}

					break;
				}
			}

			if (i == CLIENT_TOTAL_COMMANDS)
				std::cout << "Commande : inconnue" << std::endl;
		}
	}
}

bool AuthSocket::HandleLogon()
{
	std::cout << "Commande : challenge" << std::endl;
	ByteBuffer buf, packet;

	// buffer qui va recevoir les 2 octets correspondant à la taille du paquet restant
	buf.resize(2);
	handler->recv_soft((char*) buf.contents(), 2);

	uint16_t restant;
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
	packet << uint8_t(LOGON_S);
	packet << uint8_t(LOGIN_OK);
	handler->send_soft((char*) packet.contents(), packet.size());
	authed = true;

	return true;
}

bool AuthSocket::HandleRegister()
{
	std::cout << "Commande : register" << std::endl;
	ByteBuffer buf, packet;

	// buffer qui va recevoir les 2 octets correspondant à la taille du paquet restant
	buf.resize(2);
	handler->recv_soft((char*) buf.contents(), 2);

	uint16_t restant;
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
	packet << uint8_t(REGISTER_S);
	packet << uint8_t(REGISTER_OK);
	handler->send_soft((char*) packet.contents(), packet.size());

	return true;
}