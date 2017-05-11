#ifndef ROOM_H
#define ROOM_H

#include <string>

class Room {
	private:
		int roomid;
		std::string desc;

	public:
		Room(int id, std::string d) { roomid = id, desc = d; }

		std::string get_desc() { return desc; }
		int get_id() { return roomid; }
};

#endif
