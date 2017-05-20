#include "CharacterList.h"

bool CharacterList::char_exists_with_user(std::string userid) {
	for (unsigned int i = 0; i < chars.size(); i++) {
		if (chars[i].owner == userid) {
			return true;
		}
	}

	return false;
}

Character CharacterList::get_char(std::string userid) {
	for (unsigned int i = 0; i < chars.size(); i++) {
		if (chars[i].owner == userid) {
			return chars[i];
		}
	}

	throw "CharacterList::get_char called with a userid that was not found";
}

int CharacterList::get_loc(std::string userid) {
	for (unsigned int i = 0; i < chars.size(); i++) {
		if (chars[i].owner == userid) {
			return chars[i].loc;
		}
	}

	throw "CharacterList::get_loc called with a userid that was not found";
}

void CharacterList::set_loc(std::string userid, int loc) {
	for (unsigned int i = 0; i < chars.size(); i++) {
		if (chars[i].owner == userid) {
			chars[i].loc = loc;
			return;
		}
	}

	throw "CharacterList::set_loc called with a userid that was not found";
}

std::string CharacterList::get_name(std::string userid) {
	for (unsigned int i = 0; i < chars.size(); i++) {
		if (chars[i].owner == userid) {
			return chars[i].name;
		}
	}

	throw "CharacterList::get_name called with a userid that was not found";
}
