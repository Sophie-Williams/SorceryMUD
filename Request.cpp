#include "Request.h"

Request::Request(zmq::socket_t& socket) {
	zmq::message_t req;
	socket.recv(&req);
	userid = append_null((char*) req.data(), req.size());
	socket.recv(&req);
	content = append_null((char*) req.data(), req.size());
}

char* Request::append_null(char* bytes, int size) {
	char* new_bytes = new char[size];
	strcpy(new_bytes, bytes);
	new_bytes[size] = 0; // Put a null character at the last index
	return new_bytes;
} 

