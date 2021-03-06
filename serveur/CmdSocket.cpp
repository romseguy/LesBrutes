#include "stdafx.h"

#include "SocketHandler.h"
#include "CmdSocket.h"
#include "AuthCodes.h"
#include "ByteBuffer.h"
#include "BruteManager.h"

// Etat de la connexion client/serveur
enum eStatus
{
	STATUS_CONNECTED = 0,
	STATUS_AUTHED
};

// Structure de lien entre une commande (discriminant) et une fonction
typedef struct CmdHandler
{
	eCmd                          cmd;
	uint8_t                       status;
	bool (CmdSocket::*cmd_handler)(void);
} CmdHandler;

const CmdHandler table[] =
{
	{ LOGON_C,               STATUS_CONNECTED, &CmdSocket::HandleLogon             },
	{ REGISTER_C,            STATUS_CONNECTED, &CmdSocket::HandleRegister          },
	{ INFO_BRUTE_C,          STATUS_AUTHED,    &CmdSocket::HandleInfoBrute         },
	{ GET_IMAGE_C,           STATUS_AUTHED,    &CmdSocket::HandleGetImage          },
	{ GET_OPPONENT_C,        STATUS_AUTHED,    &CmdSocket::HandleGetOpponent       }
};

#define CLIENT_TOTAL_COMMANDS 5

void CmdSocket::OnRead()
{
	uint8_t cmd;
	bool stopEvent = false;

	while (!stopEvent)
	{
		std::cout << "Attente d'une commande du client" << std::endl;

		if (handler->recv_soft((char *) &cmd, 1))
		{
			size_t i;
			for (i = 0; i < CLIENT_TOTAL_COMMANDS; ++i)
			{
				if (table[i].cmd == cmd && (table[i].status == STATUS_CONNECTED || (authed && table[i].status == STATUS_AUTHED)))
				{
					// On appele la fonction correspondante � la commande donn�e
					if ((*this.*table[i].cmd_handler)())
						std::cout << "Commande : executee" << std::endl;
					else
						throw std::runtime_error("Commande : echec");

					break;
				}
			}

			if (i == CLIENT_TOTAL_COMMANDS)
				throw std::runtime_error("Commande : inconnue");
		}
		else
			stopEvent = true;
	}
}

bool CmdSocket::HandleLogon()
{
	std::cout << "Commande : challenge" << std::endl;
	ByteBuffer buf, packet;

	// buffer qui va recevoir les 2 octets correspondant � la taille du paquet restant
	buf.resize(2);

	if (!handler->recv_soft((char*) buf.contents(), 2)) return false;

	uint16_t restant;
	buf >> restant;

	// on recup�re le restant des donn�es
	buf.resize(restant + buf.size());

	if (!handler->recv_soft((char*) buf.contents(2), restant)) return false;

	// d�calage de 2 octets pour r�cup�rer le login car rpos est remis � 0 avec le resize
	std::string l, p;
	buf.rpos(2);
	buf >> l;
	buf >> p;

	// r�ponse
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

bool CmdSocket::HandleRegister()
{
	std::cout << "Commande : register" << std::endl;
	ByteBuffer buf, packet;
	buf.resize(2);

	if (!handler->recv_soft((char*) buf.contents(), 2)) return false;

	uint16_t restant;
	buf >> restant;
	buf.resize(restant + buf.size());

	if (!handler->recv_soft((char*) buf.contents(2), restant)) return false;

	std::string l, p;
	buf.rpos(2);
	buf >> l;
	buf >> p;

	// r�ponse
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

bool CmdSocket::HandleInfoBrute()
{
	std::cout << "Commande : info brute" << std::endl;
	ByteBuffer buf, packet;
	buf.resize(2);

	if (!handler->recv_soft((char*) buf.contents(), 2)) return false;

	uint16_t restant;
	buf >> restant;
	buf.resize(restant + buf.size());

	if (!handler->recv_soft((char*) buf.contents(2), restant)) return false;

	std::string l;
	buf.rpos(2);
	buf >> l;

	Brute* b = BruteManager::getInstance().get(l);

	// r�ponse
	packet << uint8_t(INFO_BRUTE_S);
	packet << uint16_t(5);
	packet << b->getPortraitId() << b->getLevel() << b->getHp() << b->getStrength() << b->getSpeed();

	return handler->send_soft((char*) packet.contents(), packet.size());
}

bool CmdSocket::HandleGetImage()
{
	std::cout << "Commande : get image" << std::endl;
	uint8_t id;

	if (!handler->recv_soft((char*) &id, 1)) return false;

	std::ostringstream imgPath;
	imgPath << "../contrib/brutesImages/personnages/Perso-" << static_cast<int>(id) << ".jpg";

	// il est n�cessaire de lire l'image en mode binaire pour le ByteBuffer
	std::ifstream img(imgPath.str(), std::ios::binary);

	ByteBuffer packet;
	packet << uint8_t(GET_IMAGE_S);

	// taille image
	img.seekg (0, std::ios::end);
	packet << uint16_t(img.tellg());
	img.seekg (0, std::ios::beg);

	packet << img;

	return handler->send_soft((char*) packet.contents(), packet.size());
}

bool CmdSocket::HandleGetOpponent()
{
	std::cout << "Commande : get opponent" << std::endl;
	ByteBuffer buf, packet;
	buf.resize(2);

	if (!handler->recv_soft((char*) buf.contents(), 2)) return false;

	uint16_t restant;
	buf >> restant;
	buf.resize(restant + buf.size());

	if (!handler->recv_soft((char*) buf.contents(2), restant)) return false;

	std::string l;
	buf.rpos(2);
	buf >> l;

	// on r�cup�re une brute du m�me niveau
	Brute* opp = BruteManager::getInstance().get(l, BruteManager::getInstance().get(l)->getLevel());

	// r�ponse
	packet << uint8_t(GET_OPPONENT_S);

	if (!opp)
	{
		packet << uint8_t(OPPONENT_KO);
		packet << uint16_t(0);
	}
	else
	{
		packet << uint8_t(OPPONENT_OK);
		packet << uint16_t(opp->getLogin().length() + 1);
		packet << opp->getLogin();
	}

	return handler->send_soft((char*) packet.contents(), packet.size());
}