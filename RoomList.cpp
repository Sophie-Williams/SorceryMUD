#include "RoomList.h"

void RoomList::init(std::string filepath) {
	std::ifstream file(filepath);
	nlohmann::json j;
	file >> j;

	for (unsigned int i = 0; i < j["rooms"].size(); i++) {
		Room r;
		r.roomid = j["rooms"][i]["roomid"];
		r.desc = j["rooms"][i]["desc"];
		rooms.push_back(r);
	}
}

std::string RoomList::get_room_desc(int r) {
	for (unsigned int i = 0; i < rooms.size(); i++) {
		if (rooms[i].roomid == r) {
			return rooms[i].desc;
		}
	}

	return "";
}
