#include "PlayerList.h"

void PlayerList::add(std::string userid, int state) {
	struct Player newplayer;
	newplayer.userid = userid;
	newplayer.state = state;
	players.push_back(newplayer);
}

void PlayerList::remove(std::string userid) {
	for (unsigned int i = 0; i < players.size(); i++) {
		if (players[i].userid == userid) {
			players.erase(players.begin() + i);
			return;
		}
	}
}

// Returns 0 if player is not connected
int PlayerList::playerstate(std::string userid) {
	for (unsigned int i = 0; i < players.size(); i++) {
		if (players[i].userid == userid) {
			return players[i].state;
		}
	}

	return 0;
}

// Returns -1 if player not found
int PlayerList::setplayerstate(std::string userid, int state) {
	for (unsigned int i = 0; i < players.size(); i++) {
		if (players[i].userid == userid) {
			players[i].state = state;
			return 0;
		}
	}
	
	return -1;
}
