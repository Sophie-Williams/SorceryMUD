#include <iostream>
#include <string>
#include <zmq.hpp>
#include <unistd.h>

#include "Server.h"

int main() {
	std::string R_PORT = "5555";
	std::string S_PORT = "9999";

	zmq::context_t context(1);
	Server server(context);
	std::cout << "Loading NPC data..." << std::endl;
	server.init_npcs("npcs.json");
	std::cout << "Loading room data..." << std::endl;
	server.init_rooms("rooms.json");
	std::cout << "Connecting to database..." << std::endl;
	server.dbconnect();
	std::cout << "Initializing server..." << std::endl;
	server.init(R_PORT, S_PORT);
	std::cout << "Listening for requests" << std::endl;

	while (true) {
		try {
			server.handle_req(std::cout);
			sleep(1); // Change this later
		}

		catch (char const* errmsg) {
			std::cout << errmsg << std::endl;
			server.send_err();
		}
	}

	return 0;
}
