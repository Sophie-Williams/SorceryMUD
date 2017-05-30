#ifndef PLAYERCHARLIST_H
#define PLAYERCHARLIST_H

#include <string>
#include <vector>

#include "PlayerChar.h"

class PlayerCharList {
	private:
		std::vector<PlayerChar> chars;

	public:
		PlayerCharList() { }
		//~PlayerCharList();

		bool char_exists_with_user(std::string); 
		void add(PlayerChar ch) { chars.push_back(ch); }
		PlayerChar get_char(std::string);
		
		int get_loc(std::string);
		void set_loc(std::string, int);
		std::string get_name(std::string);
};

#endif
