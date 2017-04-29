#include "Playerlist.h"

void Playerlist::add(std::string userid, int state) {
	struct Player newplayer;
	newplayer.userid = userid;
	newplayer.state = state;
	players.push_back(newplayer);
}

void Playerlist::remove(std::string userid) {
	for (unsigned int i = 0; i < players.size(); i++) {
		if (players[i].userid == userid) {
			players.erase(i);
			return;
		}
	}
}

// Returns 0 if player is not connected
int Playerlist::playerstate(std::string userid) {
	for (unsigned int i = 0; i < players.size(); i++) {
		if (players[i].userid == userid) {
			return players[i].state;
		}
	}

	return 0;
}

// Returns -1 if player not found
int Playerlist::setplayerstate(std::string userid, int state) {
	for (unsigned int i = 0; i < players.size(); i++) {
		if (players[i].userid == userid) {
			players[i].state = state;
			return 0;
		}
	}
	
	return -1;
}
