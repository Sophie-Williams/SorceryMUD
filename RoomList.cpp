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

Exit RoomList::get_connected_exit(int loc, int dest) {
	for (unsigned int i = 0; i < rooms.size(); i++) {
		if (rooms[i].roomid == loc) {
			for (unsigned int p = 0; p < rooms[i].exits.size(); p++) {
				if (rooms[i].exits[p].dest == dest) {
					return rooms[i].exits[p];
				}
			}

			throw "RoomList::get_connected_exit() called with unconnected rooms";
		}
	}

	throw "RoomList::get_connected_exit() called with a roomid that does not exist";
}

int RoomList::player_amnt(int roomid) {
	for (unsigned int i = 0; i < rooms.size(); i++) {
		if (rooms[i].roomid == roomid) {
			return rooms[i].players.size();
		}
	}

	return -1; 
}

PlayerChar RoomList::get_player(int roomid, int index) {
	for (unsigned int i = 0; i < rooms.size(); i++) {
		if (rooms[i].roomid == roomid) {
			if (index + 1 > int(rooms[i].players.size())) {
				throw "RoomList::get_player() called with an index that does not exist in the room's player list";
			}
			return rooms[i].players[index];
		}
	}

	throw "RoomList::get_player() called with a roomid that does not exist"; 
}

void RoomList::add_player(int roomid, PlayerChar ch) {
	for (unsigned int i = 0; i < rooms.size(); i++) {
		if (rooms[i].roomid == roomid) {
			rooms[i].players.push_back(ch);
			return;
		}
	}

	throw "RoomList::add_player() called with invalid roomid";
}

void RoomList::remove_player(int roomid, std::string userid) {
	for (unsigned int i = 0; i < rooms.size(); i++) {
		if (rooms[i].roomid == roomid) {
			for (unsigned int j = 0; j < rooms[i].players.size(); j++) {
				if (rooms[i].players[j].owner == userid) {
					rooms[i].players.erase(rooms[i].players.begin() + j);
					return;
				}
			}
		}
	}

	throw "RoomList::remove_player() called with invalid room or character name";
}
