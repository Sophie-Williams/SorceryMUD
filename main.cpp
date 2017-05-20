#include <iostream>
#include <string>
#include <zmq.hpp>
#include <unistd.h>

#include "Server.h"

int main() {
	std::string port = "5555";

	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	Server server;
	std::cout << "Loading game data..." << std::endl;
	server.init_rooms("rooms.json");
	std::cout << "Connecting to database..." << std::endl;
	server.dbconnect();
	std::cout << "Binding socket to port " + port + "..." << std::endl;
	server.bind(socket, port);
	std::cout << "Listening for requests" << std::endl;

	while (true) {
   		try {
			server.handle_req(socket);
			sleep(1); // Change this later
		}

		catch (char const* errmsg) {
			std::cout << errmsg << std::endl;
			server.send_err(socket);
		}
	}

	return 0;
}
