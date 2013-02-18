#ifndef BRUTEMANAGER_H
#define BRUTEMANAGER_H

#include "Brute.h"

class BruteManager
{
	public:
		static BruteManager& getInstance()
		{
			static BruteManager instance;
			return instance;
		}

		virtual ~BruteManager()
		{
			for (std::vector<Brute*>::iterator it = brutes.begin(); it != brutes.end(); ++it)
				delete *it;
		}

		void add(Brute* b) { brutes.push_back(b); }

		Brute* get(const std::string l) const
		{
			for (std::vector<Brute*>::const_iterator it = brutes.begin(); it != brutes.end(); ++it)
				if ((*it)->getLogin() == l)
					return *it;

			return NULL;
		}

		Brute* get(const std::string l, uint8_t level) const
		{
			for (std::vector<Brute*>::const_iterator it = brutes.begin(); it != brutes.end(); ++it)
				if ((*it)->getLogin() != l && (*it)->getLevel() == level)
					return *it;

			return NULL;
		}

	private:
		BruteManager() {};
		BruteManager(BruteManager const&);
		void operator=(BruteManager const&);

		std::vector<Brute*> brutes;
};

#endif
