#ifndef ROOMLIST_H
#define ROOMLIST_H

#include <string>
#include <vector>
#include <fstream>
#include "json.hpp"

#include "Room.h"
#include "PlayerChar.h"
#include "NonPlayerChar.h"

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

		void add_npc(int, NonPlayerChar);
};

#endif
