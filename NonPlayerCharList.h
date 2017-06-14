#ifndef NONPLAYERCHARLIST_H
#define NONPLAYERCHARLIST_H

#include <string>
#include <vector>

#include "NonPlayerChar.h"

class NonPlayerCharList {
	private:
		std::vector<NonPlayerChar> chars;
		
	public:
		//~NonPlayerCharList();

		void add_npc(NonPlayerChar npc) { chars.push_back(npc); }
};

#endif
