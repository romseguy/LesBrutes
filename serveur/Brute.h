#ifndef BRUTE_H
#define BRUTE_H

class Brute
{
	public:
		Brute(std::string login, std::string password, uint8_t level):login(login), password(password), level(level) {}
		virtual ~Brute() {}

		std::string getLogin() const { return login; }
		bool isValidPassword(std::string const& p) const { return password.compare(p) == 0; }

	private:
		std::string login, password;
		uint8_t level;
};

#endif