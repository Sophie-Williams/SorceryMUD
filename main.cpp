#include <iostream>
#include <string>
#include <zmq.hpp>
#include "json.hpp"
#include <unistd.h>

#include "Request.h"
#include "Response.h"
#include "Playerlist.h"
#include "NewCharacterList.h"
#include "Server.h"

int main() {
	std::string port = "5555";

	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	std::cout << "Initializing server" << std::endl;
	Server server;
	std::cout << "Connecting to database" << std::endl;
	server.dbconnect();
	std::cout << "Binding socket to localhost:5555" << std::endl;
	server.bind(socket, port);
	std::cout << "Listening for requests" << std::endl;

    while (true) {
		server.handle_req(socket);
		sleep(1); // Wait before running the loop again. In the future, this should be changed
	}

	return 0;
}
