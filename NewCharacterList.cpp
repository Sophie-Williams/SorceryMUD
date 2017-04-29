#include "NewCharacterList.h"

bool NewCharacterList::new_char_exists_with_owner(std::string userid) {
	for (unsigned int i = 0; i < new_characters.size(); i++) {
		if (new_characters[i].owner == userid) {
			return true;
		}
	}

	return false;
}

void NewCharacterList::add(std::string userid) {
	if (!new_char_exists_with_owner(userid)) {
		struct NewCharacter newchar;
		newchar.owner = userid;
		new_characters.push_back(newchar);
	}
}

void NewCharacterList::set_name(std::string userid, std::string name) {
	for (unsigned int i = 0; i < new_characters.size(); i++) {
		if (new_characters[i].owner == userid) {
			new_characters[i].name = name;
			break;
		}
	}
}

void NewCharacterList::set_gender(std::string userid, std::string gender) {
	for (unsigned int i = 0; i < new_characters.size(); i++) {
		if (new_characters[i].owner == userid) {
			new_characters[i].gender = gender;
			break;
		}
	}
}

void NewCharacterList::set_race(std::string userid, std::string race) {

}

void NewCharacterList::set_class(std::string userid, std::string game_class) {

}
