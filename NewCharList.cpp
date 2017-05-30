#include "NewCharList.h"

bool NewCharList::new_char_exists_with_owner(std::string userid) {
	for (unsigned int i = 0; i < new_chars.size(); i++) {
		if (new_chars[i].owner == userid) {
			return true;
		}
	}

	return false;
}

void NewCharList::add(std::string userid) {
	if (!new_char_exists_with_owner(userid)) {
		struct NewChar newchar;
		newchar.owner = userid;
		new_chars.push_back(newchar);
	}
}

void NewCharList::set_name(std::string userid, std::string name) {
	for (unsigned int i = 0; i < new_chars.size(); i++) {
		if (new_chars[i].owner == userid) {
			new_chars[i].name = name;
			return;
		}
	}
}

void NewCharList::set_gender(std::string userid, std::string gender) {
	for (unsigned int i = 0; i < new_chars.size(); i++) {
		if (new_chars[i].owner == userid) {
			new_chars[i].gender = gender;
			return;
		}
	}
}

void NewCharList::set_race(std::string userid, std::string race) {
	race[0] = toupper(race[0]);

	for (unsigned int i = 0; i < new_chars.size(); i++) {
		if (new_chars[i].owner == userid) {
			new_chars[i].race = race;
			return;
		}
	}
}

void NewCharList::set_class(std::string userid, std::string game_class) {
	game_class[0] = toupper(game_class[0]);
	
	for (unsigned int i = 0; i < new_chars.size(); i++) {
		if (new_chars[i].owner == userid) {
			new_chars[i].game_class = game_class;
			return;
		}
	}
}

std::string NewCharList::get_name(std::string userid) {
	for (unsigned int i = 0; i < new_chars.size(); i++) {
		if (new_chars[i].owner == userid) {
			return new_chars[i].name;
		}
	}

	return nullptr;
}

std::string NewCharList::get_gender(std::string userid) {
	for (unsigned int i = 0; i < new_chars.size(); i++) {
		if (new_chars[i].owner == userid) {
			return new_chars[i].gender;
		}
	}

	return nullptr;
}

std::string NewCharList::get_race(std::string userid) {
	for (unsigned int i = 0; i < new_chars.size(); i++) {
		if (new_chars[i].owner == userid) {
			return new_chars[i].race;
		}
	}

	return nullptr;
}

std::string NewCharList::get_class(std::string userid) {
	for (unsigned int i = 0; i < new_chars.size(); i++) {
		if (new_chars[i].owner == userid) {
			return new_chars[i].game_class;
		}
	}

	return nullptr;
}

