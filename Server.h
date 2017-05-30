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
#include <vector> // To hold words

#include "Request.h"
#include "Response.h"
#include "PlayerList.h"
#include "NewCharList.h"
#include "RoomList.h"
#include "PlayerCharList.h"

class Server {
	private:
		std::string server_addr = "127.0.0.1";
		zmq::socket_t r_socket;
		zmq::socket_t s_socket;

		PlayerList connected;
		NewCharList newchars;
		RoomList rooms;
		PlayerCharList chars;

		PGconn *conn;

		void dbcommand(std::string);
		PGresult* dbselect(std::string);

		std::string CMD_INVALID;

	public:
		Server(zmq::context_t&);

		std::string look(std::string);
		std::string look_roomid(std::string, int);

		std::string in_game(std::string, std::string);
		std::string main_menu(std::string, std::string);
		std::string not_connected(std::string, std::string);
		std::string menu(std::string, std::string);
		std::string naming(std::string, std::string);
		std::string select_gender(std::string, std::string);
		std::string select_race(std::string, std::string);
		std::string select_class(std::string, std::string);
		std::string newchar_confirm(std::string, std::string);

		void move_char(std::string, int, int);
		void init_rooms(std::string filepath) { rooms.init(filepath); }
		void load_char(std::string, std::string);
		void init(std::string r_port, std::string s_port);
		void dbconnect();

		void send_err();
		void handle_req();
		void handle_req(std::ostream&);
		void notify(std::vector<std::string>&, std::string);
		void notify_room(int, std::string);
};

#endif
