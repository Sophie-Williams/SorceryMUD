#ifndef PLAYERCHAR_H
#define PLAYERCHAR_H

#include <string>

#include "Character.h"

class PlayerChar: public Character {
	public:
		std::string owner;
		std::string game_class;
		int level;
		int xp;
		// attributes
		int loc;
		int money;
		// inventory
		// abilities
		// spells
		// status conditions
};

#endif
