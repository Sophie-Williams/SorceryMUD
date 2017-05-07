#ifndef SERVER_H
#define SERVER_H

#include <sstream> // Not needed yet
#include <iostream>
#include <string>
#include <algorithm>
#include <zmq.hpp>
#include <libpq-fe.h>

#include "Request.h"
#include "Response.h"
#include "Playerlist.h"
#include "NewCharacterList.h"

class Server {
	private:
		Playerlist connected;
		NewCharacterList newchars;

		PGconn *conn;

		void dbcommand(std::string);
		PGresult* dbselect(std::string);

		std::string CMD_INVALID;

	public:
		Server();

		std::string main_menu(std::string, std::string);
		std::string not_connected(std::string, std::string);
		std::string menu(std::string, std::string);
		std::string naming(std::string, std::string);
		std::string select_gender(std::string, std::string);
		std::string select_race(std::string, std::string);
		std::string select_class(std::string, std::string);
		std::string newchar_confirm(std::string, std::string);

		void bind(zmq::socket_t& socket, std::string port) { socket.bind("tcp://*:" + port); }
		void dbconnect();

		void handle_req(zmq::socket_t&);
		void handle_req(zmq::socket_t&, std::ostream&);
};

#endif
