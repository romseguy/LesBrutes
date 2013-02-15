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
			speed(speed)
			{}

		virtual ~Brute() {}

		std::string getLogin() const { return login; }
		uint8_t getLevel() const { return level; }
		uint8_t getHp() const { return hp; }
		uint8_t getStrength() const { return strength; }
		uint8_t getSpeed() const { return speed; }

		bool isValidPassword(std::string const& p) const { return password.compare(p) == 0; }

	private:
		std::string login, password;
		uint8_t level, hp, strength, speed;
};

#endif