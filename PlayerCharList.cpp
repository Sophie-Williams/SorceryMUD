#include "PlayerCharList.h"

bool PlayerCharList::char_exists_with_user(std::string userid) {
	for (unsigned int i = 0; i < chars.size(); i++) {
		if (chars[i].owner == userid) {
			return true;
		}
	}

	return false;
}

PlayerChar PlayerCharList::get_char(std::string userid) {
	for (unsigned int i = 0; i < chars.size(); i++) {
		if (chars[i].owner == userid) {
			return chars[i];
		}
	}

	throw "PlayerCharList::get_char called with a userid that was not found";
}

int PlayerCharList::get_loc(std::string userid) {
	for (unsigned int i = 0; i < chars.size(); i++) {
		if (chars[i].owner == userid) {
			return chars[i].loc;
		}
	}

	throw "PlayerCharList::get_loc called with a userid that was not found";
}

void PlayerCharList::set_loc(std::string userid, int loc) {
	for (unsigned int i = 0; i < chars.size(); i++) {
		if (chars[i].owner == userid) {
			chars[i].loc = loc;
			return;
		}
	}

	throw "PlayerCharList::set_loc called with a userid that was not found";
}

std::string PlayerCharList::get_name(std::string userid) {
	for (unsigned int i = 0; i < chars.size(); i++) {
		if (chars[i].owner == userid) {
			return chars[i].name;
		}
	}

	throw "PlayerCharList::get_name called with a userid that was not found";
}
