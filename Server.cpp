#include "Server.h"

void Server::bind(std::string port) { 
	context = zmq::context_t(1);
	socket = zmq::socket_t(context, ZMQ_REP);
	s << "Binding socket to localhost:" << port << std::endl;
	socket.bind("tcp://*:" + port);
	s << "Listening for requests on port " << port << std::endl;
}

void Server::handle_req() {
	Request req(socket);
	Request rep;

	std::string userid = req.get_userid();
	std::string content = req.get_content();
	s << "Received message from user " << userid << ": '" << content << "'" << std::endl;

	int state = connected.playerstate(userid);

	if (state == 0) {
		if (content == "connect") {
			
}
