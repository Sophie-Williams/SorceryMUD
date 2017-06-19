// This class basically represents any creature (sentient or unsentient) in the game world. Subclasses of Character are PlayerChar and NonPlayerChar.

#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

#include "Lookable.h"

class Character: public Lookable {
	public:
		char gender;
		std::string race;
		int hp_cur;
		int hp_max;
};

#endif
