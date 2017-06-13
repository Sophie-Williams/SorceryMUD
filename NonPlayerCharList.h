#ifndef NONPLAYERCHARLIST_H
#define NONPLAYERCHARLIST_H

#include <string>
#include <vector>
#include <fstream>
#include "json.hpp"

#include "NonPlayerChar.h"

class NonPlayerCharList {
	private:
		std::vector<NonPlayerChar> npctypes; // It may be necessary to create a new NPCType class for this, but not yet
		std::vector<NonPlayerChar> chars;
	public:
		//~NonPlayerCharList();

		void init(std::string);
};

#endif
