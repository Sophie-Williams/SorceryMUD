#include "Request.h"
#include "Response.h"

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
		Response rep;

		std::cout << "Received message from user " << req.get_userid() << ": '" << req.get_content() << "'" << std::endl;
		//  Process the message 
		if (req.get_content() == "connect") {
			// It's impossible to connect through the Discord bot while already connected.
			// However, there is a possibility that someone could send messages through other means and bypass the restrictions placed by the client.
			// The user should only be allowed to connect if their ID is not in connected.
			connected.push_back(req.get_userid());
			// Retrieve character data for the user, and display it
			// Provide options to select, create, or delete a character
			rep.set("Welcome to SorceryMUD."); // Insert ASCII art here
		}

		else if (req.get_content() == "disconnect") {
			// Save player data to the database
			// Remove player from connected list
			rep.set(""); // "Dummy" response to preserve req-rep pattern
		}

		else {
			rep.set("You have been pwned by a grue.");
		}

		rep.send(socket);

		sleep(1); // Wait before running the loop again. In the future, this should measure the amount of time elapsed since the beginning of the loop, and based on the tickrate, wait a certain amount of time, adjusted for the previous measurement
	}

	return 0;
}
