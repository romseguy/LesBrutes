#ifndef BRUTE_H
#define BRUTE_H

class Brute
{
	public:
		Brute() {}
		virtual ~Brute() {}

	private:
		std::string login, password;
		uint8_t level;
};

#endif