// NewCharacter struct and NewCharacterList class for SorceryMUD, assists in keeping track of new character info when they are being created

#ifndef NEWCHARACTERLIST_H
#define NEWCHARACTERLIST_H

#include <string>
#include <vector>

struct NewCharacter {
	std::string owner;
	std::string name;
	std::string gender;
	std::string race;
	std::string game_class;
};

class NewCharacterList {
	private:
		std::vector<NewCharacter> new_characters;

		bool new_char_exists_with_owner(std::string);

	public:
		NewCharacterList() { }
		//~NewCharacterList();

		void add(std::string);
		void set_name(std::string, std::string);
		void set_gender(std::string, std::string);
		void set_race(std::string, std::string);
		void set_class(std::string, std::string);
};

#endif
