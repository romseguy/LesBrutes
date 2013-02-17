#ifndef BRUTE_H
#define BRUTE_H

class Brute
{
	public:
		Brute(std::string login, std::string password, uint8_t level, uint8_t hp, uint8_t strength, uint8_t speed):
			login(login),
			password(password),
			level(level),
			hp(hp),
			strength(strength),
			speed(speed),
			portraitImg(NULL)
			{
				srand(time(NULL));
				portraitId = rand() % 13 + 1;
			}

		Brute(std::string login, std::string password, uint8_t level, uint8_t hp, uint8_t strength, uint8_t speed, const uint8_t* portraitImg):
			login(login),
			password(password),
			level(level),
			hp(hp),
			strength(strength),
			speed(speed),
			portraitImg(portraitImg)
			{}

		virtual ~Brute() { if (portraitImg) delete portraitImg; }

		std::string getLogin() const  { return login; }
		uint8_t getPortraitId() const { return portraitId; }
		uint8_t getLevel() const      { return level; }
		uint8_t getHp() const         { return hp; }
		uint8_t getStrength() const   { return strength; }
		uint8_t getSpeed() const      { return speed; }

		const uint8_t* getPortrait() const { return portraitImg; }

		bool isValidPassword(std::string const& p) const { return password.compare(p) == 0; }

	private:
		std::string login, password;
		uint8_t portraitId, level, hp, strength, speed;
		const uint8_t* portraitImg;
};

#endif