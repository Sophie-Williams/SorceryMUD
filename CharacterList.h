#ifndef CHARACTERLIST_H
#define CHARACTERLIST_H

#include <string>
#include <vector>

struct Character {
	std::string owner;
	std::string name;
	char gender;
	std::string race;
	std::string game_class;
	int level;
	int xp;
	// attributes
	int loc;
	int hp_cur;
	int hp_max;
	int money;
	// inventory
	// abilities
	// spells
	// status conditions
};

class CharacterList {
	private:
		std::vector<Character> chars;

	public:
		CharacterList() { }
		//~CharacterList();

		bool char_exists_with_user(std::string); 
		void add(Character ch) { chars.push_back(ch); }

		int get_loc(std::string);
		void set_loc(std::string, int);
};

#endif
