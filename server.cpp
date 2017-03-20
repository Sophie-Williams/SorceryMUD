#include "Request.h"

#include <zmq.hpp>
#include <string>
#include <vector>
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
	std::cout << "Listening for requests" << std::endl;

	std::vector<std::string> connected; // Holds the IDs of all users who are connected to the game server
	
    while (true) {
		Request req(socket);
		std::string result;

		std::cout << "Received message from user " << req.get_userid() << ": '" << req.get_content() << "'" << std::endl;
		//  Process the message 
		if (req.get_content() == "connect") {
			connected.push_back(req.get_userid());
			// Retrieve character data for the user, and display it
			// Provide options to select, create, or delete a character
			result = "Welcome to SorceryMUD."; // Insert ASCII art here
		}

		else if (req.get_content() == "disconnect") {
			// Save player data to the database and remove player from connected list 
			//result = "";
		}

		else {
			result = "You have been pwned by a grue.";
		}

		//  Send reply to client
		zmq::message_t reply(result.length()); // Hopefully I can make this process OO soon
		memcpy(reply.data(), result.data(), result.length());
		socket.send(reply);

		sleep(1); // Wait before running the loop again. In the future, this should measure the amount of time elapsed since the beginning of the loop, and based on the tickrate, wait a certain amount of time, adjusted for the previous measurement
	}

	return 0;
}
