#include "Response.h"

void Response::send(zmq::socket_t& socket) {
	zmq::message_t rep(content.length());
	memcpy(rep.data(), content.data(), content.length());
	socket.send(rep);
}

void Response::send_multipart(zmq::socket_t& socket) {
	zmq::message_t rep(content.length());
	memcpy(rep.data(), content.data(), content.length());
	socket.send(rep, ZMQ_SNDMORE);
}
