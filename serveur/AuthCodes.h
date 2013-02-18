#ifndef AUTHCODES_H
#define AUTHCODES_H

// Discriminant de message
enum eCmd
{
	LOGON_C                       = 0x00,
	LOGON_S                       = 0x01,
	REGISTER_C                    = 0x02,
	REGISTER_S                    = 0x03,
	INFO_BRUTE_C                  = 0x04,
	INFO_BRUTE_S                  = 0x05,
	GET_IMAGE_C                   = 0x06,
	GET_IMAGE_S                   = 0x07,
	GET_OPPONENT_C                = 0x08,
	GET_OPPONENT_S                = 0x09
};

// Codes d'erreur
enum LoginResult
{
	LOGIN_OK                      = 0x00,
	LOGIN_KO                      = 0x01
};

enum RegisterResult
{
	REGISTER_OK                   = 0x00,
	REGISTER_KO                   = 0x01,
	REGISTER_ALREADY_EXISTS       = 0x02
};

enum OpponentResult
{
	OPPONENT_OK                   = 0x00,
	OPPONENT_KO                   = 0x01
};

/**************************************************
 * Messages échangés entre le client et le serveur
 * A titre indicatif, les structures ne sont pas
 * utilisées directement pour l'envoi des messages
 **************************************************/
struct LOGON_C // REGISTER_C
{
	uint8_t                       cmd;
	uint16_t                      size;
	char                          login;
	char                          pwd;
};

struct LOGON_S // REGISTER_S
{
	uint8_t                       cmd;
	uint8_t                       error;
};

struct INFO_BRUTE_C // GET_OPPONENT_C
{
	uint8_t                       cmd;
	uint16_t                      size;
	char                          login;
};

struct INFO_BRUTE_S
{
	uint8_t                       cmd;
	uint16_t                      size;
	uint8_t                       portraitId;
	uint8_t                       level;
	uint8_t                       hp;
	uint8_t                       strength;
	uint8_t                       speed;
};

struct GET_IMAGE_C
{
	uint8_t                       cmd;
	uint8_t                       id;
};

struct GET_IMAGE_S
{
	uint8_t                       cmd;
	uint16_t                      size;
	uint8_t                       img;
};

struct GET_OPPONENT_S
{
	uint8_t                       cmd;
	uint8_t                       error;
	uint16_t                      size;
	char                          login;
};

#endif