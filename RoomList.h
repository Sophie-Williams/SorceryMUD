#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <fstream>
#include "json.hpp"

struct Room {
	int roomid;
	std::string desc;
};

class RoomList {
	private:
		std::vector<Room> rooms;

	public:
		void init(std::string);
		std::string get_room_desc(int);
};

#endif
