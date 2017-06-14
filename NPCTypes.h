#ifndef NPCTYPE_H
#define NPCTYPE_H

#include <string>
#include <vector>
#include <fstream>
#include "json.hpp"

#include "NonPlayerChar.h"

class NPCTypes {
	private:
		std::vector<NonPlayerChar> npctypes;
		
	public:
		//~NonPlayerCharList();

		void init(std::string);
		void add_npctype(NonPlayerChar npc) { npctypes.push_back(npc); }
		NonPlayerChar get_npctype(std::string);
};

#endif
