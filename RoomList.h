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
		//void init(std::string);
		void add_room(Room room) { rooms.push_back(room); }
		Room* get_room(int roomid);
		std::string get_room_desc(int);
		int exit_amnt(int);
		Exit get_exit(int, int);
		Exit get_connected_exit(int, int);

		int player_amnt(int);
		PlayerChar get_player(int roomid, int index);
		void add_player(int, PlayerChar);
		void remove_player(int, std::string);

		int npc_amnt(int);
		NonPlayerChar get_npc(int, int);
		void add_npc(int, NonPlayerChar);
};

#endif
