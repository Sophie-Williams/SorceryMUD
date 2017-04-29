#include "Playerlist.h"

// Returns 0 if player is not connected
int Playerlist::playerstate(std::string playerid) {
	for (unsigned int i = 0; i < players.size(); i++) {
		if (players[i].get_id() == playerid) {
			return players[i].get_state();
		}
	}

	return 0;
}

void Playerlist::setplayerstate(std::string playerid, int newstate) {
	for (unsigned int i = 0; i < players.size(); i++) {
		if (players[i].get_id() == playerid) {
			players[i].set_state(newstate);
		}

		break;
	}
}
