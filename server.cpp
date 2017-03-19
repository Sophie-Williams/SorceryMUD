#include <zmq.hpp>
#include <string>
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>

#define sleep(n)    Sleep(n)
#endif

int main() {
	std::cout << "SorceryMUD, a MUD game created to demonstrate the potential of MUDBot" << std::endl;

    zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	std::cout << "Binding socket to localhost:5555 ... " << std::endl;
    socket.bind("tcp://*:5555");
	std::cout << "Listening for connections" << std::endl;

    while (true) {
		// Wait for a message
		zmq::message_t request;
		socket.recv(&request);
		int size = request.size();
		char* msg = new char[size];
		strcpy(msg, (char*) request.data());
		msg[size] = 0; // It's necessary to append a null terminator, since the string obtained from data() is not guaranteed to have one
		std::cout << "Received message: '" << msg << "'" << std::endl; // Ideally I'd like to use std::string as much as possible, but there's no point here because I'd just have to convert from the pointer to char* to std::string with no benefit

		//  Process the command

		//  Send reply to client
		zmq::message_t reply(30);
		memcpy(reply.data(), "You have been pwned by a grue." , 30);
		socket.send(reply);

		sleep(1); // Wait before running the loop again. In the future, this should measure the amount of time elapsed since the beginning of the loop, and based on the tickrate, wait a certain amount of time, adjusted for the previous measurement
	}

	return 0;
}
