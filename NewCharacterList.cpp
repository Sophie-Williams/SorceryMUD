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
			return;
		}
	}
}

void NewCharacterList::set_gender(std::string userid, std::string gender) {
	for (unsigned int i = 0; i < new_characters.size(); i++) {
		if (new_characters[i].owner == userid) {
			new_characters[i].gender = gender;
			return;
		}
	}
}

void NewCharacterList::set_race(std::string userid, std::string race) {
	for (unsigned int i = 0; i < new_characters.size(); i++) {
		if (new_characters[i].owner == userid) {
			new_characters[i].race = race;
			return;
		}
	}
}

void NewCharacterList::set_class(std::string userid, std::string game_class) {
	for (unsigned int i = 0; i < new_characters.size(); i++) {
		if (new_characters[i].owner == userid) {
			new_characters[i].game_class = game_class;
			return;
		}
	}
}

std::string NewCharacterList::get_name(std::string userid) {
	for (unsigned int i = 0; i < new_characters.size(); i++) {
		if (new_characters[i].owner == userid) {
			return new_characters[i].name;
		}
	}

	return nullptr;
}

std::string NewCharacterList::get_gender(std::string userid) {
	for (unsigned int i = 0; i < new_characters.size(); i++) {
		if (new_characters[i].owner == userid) {
			return new_characters[i].gender;
		}
	}

	return nullptr;
}

std::string NewCharacterList::get_race(std::string userid) {
	for (unsigned int i = 0; i < new_characters.size(); i++) {
		if (new_characters[i].owner == userid) {
			return new_characters[i].race;
		}
	}

	return nullptr;
}

std::string NewCharacterList::get_class(std::string userid) {
	for (unsigned int i = 0; i < new_characters.size(); i++) {
		if (new_characters[i].owner == userid) {
			return new_characters[i].game_class;
		}
	}

	return nullptr;
}

