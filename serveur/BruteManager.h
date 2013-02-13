#ifndef BRUTEMANAGER_H
#define BRUTEMANAGER_H

class Brute;

class BruteManager
{
	public:
		BruteManager() {}
		virtual ~BruteManager() {}

	private:
		std::vector<Brute> brutes;
};

#endif
