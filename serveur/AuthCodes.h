#ifndef _AUTHCODES_H
#define _AUTHCODES_H

// Discriminant (commande)
enum eCmd
{
	LOGON_C                       = 0x00,
	LOGON_S                       = 0x01,
	REGISTER_C                    = 0x02,
	REGISTER_S                    = 0x03,
};

enum LoginResult
{
	LOGIN_OK                                     = 0x00,
	LOGIN_KO                                     = 0x01
};

enum RegisterResult
{
	REGISTER_OK                                  = 0x00,
	REGISTER_KO                                  = 0x01
};

#endif