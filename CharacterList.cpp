#include "CharacterList.h"

bool CharacterList::char_exists_with_user(std::string userid) {
	for (unsigned int i = 0; i < chars.size(); i++) {
		if (chars[i].owner == userid) {
			return true;
		}
	}

	return false;
}

int CharacterList::get_loc(std::string userid) {
	for (unsigned int i = 0; i < chars.size(); i++) {
		if (chars[i].owner == userid) {
			return chars[i].loc;
		}
	}

	return -1;
}

void CharacterList::set_loc(std::string userid, int loc) {
	for (unsigned int i = 0; i < chars.size(); i++) {
		if (chars[i].owner == userid) {
			chars[i].loc = loc;
		}
	}
}
