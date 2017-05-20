#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <fstream>
#include "json.hpp"

#include "CharacterList.h"

struct Exit {
	std::string name;
	int dest;
};

struct Room {
	int roomid;
	std::string desc;
	std::vector<Character> players;
	std::vector<Exit> exits;
};

class RoomList {
	private:
		std::vector<Room> rooms;

	public:
		void init(std::string);
		std::string get_room_desc(int);
		int exit_amnt(int);
		struct Exit get_exit(int, int);
		int player_amnt(int);
		Character get_player(int, int);
		void add_player(int, Character);
		void remove_player(int, std::string);
};

#endif
