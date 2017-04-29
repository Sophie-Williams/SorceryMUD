// Player class for SorceryMUD, assists in keeping track of players who are connected

#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
	private:
		std::string playerid;
		int state;

	public:
		Player(std::string id) { playerid = id; state = 0; }
		Player(std::string id, int s) { playerid = id; state = s; }

		void set_state(int s) { state = s; }
		std::string get_id() { return playerid; }
		int get_state() { return state; }
};

#endif
