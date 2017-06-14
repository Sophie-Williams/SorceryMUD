#include "NPCTypes.h"

void NPCTypes::init(std::string filepath) {
	std::ifstream file(filepath);
	nlohmann::json npcdata;
	file >> npcdata;

	for (auto it = npcdata["npcs"].begin(); it != npcdata["npcs"].end(); ++it) {
		NonPlayerChar npc;
		npc.internal = it.value()["name"];
		npc.name = it.value()["display"];
		npc.desc = it.value()["desc"];
		npctypes.push_back(npc);
	}
}

NonPlayerChar NPCTypes::get_npctype(std::string internal) {
	// Iterators?
	for (size_t i = 0; i < npctypes.size(); i++) {
		if (npctypes[i].internal == internal) {
			return npctypes[i];
		}
	}

	throw "NPCTypes::get_npctype called with an invalid argument";
}