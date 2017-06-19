#include "Room.h"

Exit Room::get_exit_by_roomid(int dest) {
	for (size_t p = 0; p < exits.size(); p++) {
		if (exits[p].dest == dest) {
			return exits[p];
		}
	}

	throw "Room::get_exit_by_roomid() called with invalid roomid";
}

void Room::remove_player(std::string userid) {
	for (size_t j = 0; j < players.size(); j++) {
		if (players[j].owner == userid) {
			players.erase(players.begin() + j);
			return;
		}
	} 

	throw "No player character was found with that user ID in Room::remove_player()";
}

Lookable Room::get_lookable(std::string alias) {
	for (auto it = players.begin(); it != players.end(); ++it) {
		if (it->matches_alias(alias)) {
			return *it;
		}
	}

	for (auto it = npcs.begin(); it != npcs.end(); ++it) {
		if (it->matches_alias(alias)) {
			return *it;
		}
	}
	
	for (auto it = exits.begin(); it != exits.end(); ++it) {
		if (it->matches_alias(alias)) {
			return *it;
		}
	}

	throw "Couldn't find that thing.";
}
