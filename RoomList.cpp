#include "RoomList.h"

void RoomList::init(std::string filepath) {
	std::ifstream file(filepath);
	nlohmann::json roomdata;
	file >> roomdata;

	for (unsigned int i = 0; i < roomdata["rooms"].size(); i++) {
		Room room;
		room.roomid = roomdata["rooms"][i]["roomid"];
		room.desc = roomdata["rooms"][i]["desc"];
		for (unsigned int j = 0; j < roomdata["rooms"][i]["exits"].size(); j++) {
			Exit exit;
			exit.name = roomdata["rooms"][i]["exits"][j][0];
			exit.dest = roomdata["rooms"][i]["exits"][j][1];
			room.exits.push_back(exit);
		}
		rooms.push_back(room);
	}
}

std::string RoomList::get_room_desc(int roomid) {
	for (unsigned int i = 0; i < rooms.size(); i++) {
		if (rooms[i].roomid == roomid) {
			return rooms[i].desc;
		}
	}

	return "";
}

int RoomList::exit_amnt(int roomid) {
	for (unsigned int i = 0; i < rooms.size(); i++) {
		if (rooms[i].roomid == roomid) {
			return rooms[i].exits.size();
		}
	}

	return -1; 
}

Exit RoomList::get_exit(int roomid, int index) {
	for (unsigned int i = 0; i < rooms.size(); i++) {
		if (rooms[i].roomid == roomid) {
			if (index + 1 > int(rooms[i].exits.size())) {
				throw "RoomList::get_exit() called with an exit number that does not exist in the room";
			}
			return rooms[i].exits[index];
		}
	}

	throw "RoomList::get_exit() called with a roomid that does not exist"; 
}

