// This class contains some stuff specific to NPCs, for example, functionality for keeping track of simultaneous conversations with multiple PCs, aggro data, etc.

#ifndef NONPLAYERCHAR_H
#define NONPLAYERCHAR_H

#include "Character.h"

class NonPlayerChar: public Character {
	public:
		std::string internal; // The internal name in the JSON file
		// The "name" attribute corresponds to the "display" field in the JSON file
		std::string desc; // The text that represents the NPC when you type "look"
};

#endif
