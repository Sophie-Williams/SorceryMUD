#ifndef SERVER_H
#define SERVER_H

#include "Request.h"
#include "Response.h"

#include <iostream>
#include <string>
#include <locale>
#include <zmq.hpp>
#include <libpq-fe.h>

class Server {
	private:
		std::ostream s;

		Playerlist connected;
		NewCharacterList newchars;

		zmq::context_t context;
		zmq::socket_t socket;

	public:
		Server() { s = std::cout; }
		Server(std::ostream ostream) { s = ostream; }

		void bind(std::string);
		void handle_req();
};

#endif
