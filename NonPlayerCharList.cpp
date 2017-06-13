#include "NonPlayerCharList.h"

void NonPlayerCharList::init(std::string filepath) {
	std::ifstream file(filepath);
	nlohmann::json npcdata;
	file >> npcdata;

	for (auto it = npcdata["npcs"].begin(); it != npcdata["npcs"].end(); ++it) {
		NonPlayerChar npc;
		// The internal name will be important soon
		npc.name = it.value()["display"];
		npctypes.push_back(npc);
	}
}
