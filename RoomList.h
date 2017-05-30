#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <fstream>
#include "json.hpp"

#include "PlayerCharList.h"

struct Exit {
	std::string name;
	int dest;
};

struct Room {
	int roomid;
	std::string desc;
	std::vector<PlayerChar> players;
	std::vector<Exit> exits;
};

class RoomList {
	private:
		std::vector<Room> rooms;

	public:
		void init(std::string);
		std::string get_room_desc(int);
		int exit_amnt(int);
		Exit get_exit(int, int);
		Exit get_connected_exit(int, int);
		int player_amnt(int);
		PlayerChar get_player(int, int);
		void add_player(int, PlayerChar);
		void remove_player(int, std::string);
};

#endif
