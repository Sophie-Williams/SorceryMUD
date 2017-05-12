#ifndef SERVER_H
#define SERVER_H

#include <sstream> // Not needed yet
#include <iostream>
#include <string>
#include <algorithm>
#include <zmq.hpp>
#include <libpq-fe.h>
#include <fstream>
#include "json.hpp"

#include "Request.h"
#include "Response.h"
#include "PlayerList.h"
#include "NewCharacterList.h"
#include "RoomList.h"
#include "CharacterList.h"

class Server {
	private:
		PlayerList connected;
		NewCharacterList newchars;
		RoomList rooms;
		CharacterList chars;

		PGconn *conn;

		void dbcommand(std::string);
		PGresult* dbselect(std::string);

		std::string CMD_INVALID;

	public:
		Server();

		std::string look(std::string);
		std::string in_game(std::string, std::string);
		std::string main_menu(std::string, std::string);
		std::string not_connected(std::string, std::string);
		std::string menu(std::string, std::string);
		std::string naming(std::string, std::string);
		std::string select_gender(std::string, std::string);
		std::string select_race(std::string, std::string);
		std::string select_class(std::string, std::string);
		std::string newchar_confirm(std::string, std::string);

		void init_rooms(std::string filepath) { rooms.init(filepath); }
		void load_char(std::string, std::string);
		void bind(zmq::socket_t& socket, std::string port) { socket.bind("tcp://*:" + port); }
		void dbconnect();

		void handle_req(zmq::socket_t&);
		void handle_req(zmq::socket_t&, std::ostream&);
};

#endif
