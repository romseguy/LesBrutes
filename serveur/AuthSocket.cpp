#include "stdafx.h"

#include "SocketHandler.h"
#include "AuthSocket.h"
#include "AuthCodes.h"
#include "ByteBuffer.h"
#include "BruteManager.h"

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
	char[]                        login;
	char[]                        pwd;
};

struct LOGON_S, REGISTER_S
{
	uint8_t                       cmd;
	uint8_t                       error;
};

struct INFO_BRUTE_C
{
	uint8_t                       cmd;
	uint16_t                      size;
	char[]                        login;
};

struct INFO_BRUTE_S
{
	uint8_t                       cmd;
	uint16_t                      size;
	uint8_t                       level;
	uint8_t                       hp;
	uint8_t                       strength;
	uint8_t                       speed;
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
	{ REGISTER_C,            STATUS_CONNECTED, &AuthSocket::HandleRegister          },
	{ INFO_BRUTE_C,          STATUS_AUTHED,    &AuthSocket::HandleInfoBrute         }
};

#define CLIENT_TOTAL_COMMANDS 3

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
				std::cout << "Commande : inconnue " << std::endl;
		}
	}
}

bool AuthSocket::HandleLogon()
{
	std::cout << "Commande : challenge" << std::endl;
	ByteBuffer buf, packet;

	// buffer qui va recevoir les 2 octets correspondant à la taille du paquet restant
	buf.resize(2);

	if (!handler->recv_soft((char*) buf.contents(), 2))
		return false;

	uint16_t restant;
	buf >> restant;

	// on recupère le restant des données
	buf.resize(restant + buf.size());

	if (!handler->recv_soft((char*) buf.contents(2), restant))
		return false;

	// décalage de 2 octets pour récupérer le login car rpos est remis à 0 avec le resize
	std::string l, p;
	buf.rpos(2);
	buf >> l;
	buf >> p;

	// réponse
	packet << uint8_t(LOGON_S);

	Brute* b = BruteManager::getInstance().get(l);

	if (b && b->isValidPassword(p))
	{
		authed = true;
		packet << uint8_t(LOGIN_OK);
	}
	else
		packet << uint8_t(LOGIN_KO);

	return handler->send_soft((char*) packet.contents(), packet.size());
}

bool AuthSocket::HandleRegister()
{
	std::cout << "Commande : register" << std::endl;
	ByteBuffer buf, packet;

	// buffer qui va recevoir les 2 octets correspondant à la taille du paquet restant
	buf.resize(2);

	if (!handler->recv_soft((char*) buf.contents(), 2))
		return false;

	uint16_t restant;
	buf >> restant;

	// on recupère le restant des données
	buf.resize(restant + buf.size());

	if (!handler->recv_soft((char*) buf.contents(2), restant))
		return false;

	// décalage de 2 octets pour récupérer le login car rpos est remis à 0 avec le resize
	std::string l, p;
	buf.rpos(2);
	buf >> l;
	buf >> p;

	// réponse
	packet << uint8_t(REGISTER_S);

	if (BruteManager::getInstance().get(l))
		packet << uint8_t(REGISTER_ALREADY_EXISTS);
	else
	{
		BruteManager::getInstance().add(new Brute(l, p, 1, 100, 2, 3));
		packet << uint8_t(REGISTER_OK);
	}

	return handler->send_soft((char*) packet.contents(), packet.size());
}

bool AuthSocket::HandleInfoBrute()
{
	std::cout << "Commande : info brute" << std::endl;
	ByteBuffer buf, packet;

	// buffer qui va recevoir les 2 octets correspondant à la taille du paquet restant
	buf.resize(2);

	if (!handler->recv_soft((char*) buf.contents(), 2))
		return false;

	uint16_t restant;
	buf >> restant;

	// on recupère le restant des données
	buf.resize(restant + buf.size());

	if (!handler->recv_soft((char*) buf.contents(2), restant))
		return false;

	// décalage de 2 octets pour récupérer le login car rpos est remis à 0 avec le resize
	std::string l;
	buf.rpos(2);
	buf >> l;

	Brute* b = BruteManager::getInstance().get(l);

	// réponse
	packet << uint8_t(INFO_BRUTE_S);
	packet << uint16_t(4) << b->getLevel() << b->getHp() << b->getStrength() << b->getSpeed();

	delete b;

	return handler->send_soft((char*) packet.contents(), packet.size());
}