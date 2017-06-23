#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <exception>

#include "PlayerChar.h"
#include "NonPlayerChar.h"

class Exit: public Lookable {
	public:
		int dest;
};

class lookable_not_found: public std::exception {
	// This exception is thrown when the get_lookable function fails to find a matching Lookable object
};

class Room {
	public:
		int roomid;
		std::string desc;
		std::vector<PlayerChar> players;
		std::vector<NonPlayerChar> npcs; // Given that this list should not be used for any real processing and only really exists here as an optimization measure, I'm not really sure that it's necessary to store entire NonPlayerChar objects
		std::vector<Exit> exits;

		Exit get_exit_by_roomid(int);
		void remove_player(std::string);

		void add_npc(NonPlayerChar npc) { npcs.push_back(npc); }
		//void remove_npc(std::string);

		Lookable get_lookable(std::string alias);
};

#endif
