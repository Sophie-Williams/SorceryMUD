#include "Room.h"

Exit Room::get_exit_by_roomid(int dest) {
	for (unsigned int p = 0; p < exits.size(); p++) {
		if (exits[p].dest == dest) {
			return exits[p];
		}
	}

	throw "Room::get_exit_by_roomid() called with invalid roomid";
}

void Room::remove_player(std::string userid) {
	for (unsigned int j = 0; j < players.size(); j++) {
		if (players[j].owner == userid) {
			players.erase(players.begin() + j);
			return;
		}
	} 

	throw "No player character was found with that user ID in Room::remove_player()";
}
