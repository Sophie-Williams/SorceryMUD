#include "Request.h"
#include "Response.h"

#include <string>
#include <vector>
#include <iostream>
#include <zmq.hpp>
#include <libpq-fe.h>
//#include "json.hpp"
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
			std::string rep_msg = "Welcome to SorceryMUD.\n\n"; // Insert ASCII art here

			PGresult *res;
			PGconn *conn = PQconnectdb("host=localhost dbname=sorcery user=sorcery password=ryu5g7cwq89t97z5t4yq");
			if (PQstatus(conn) != CONNECTION_OK) {
				fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
				PQfinish(conn);
				exit(1);
			}

			std::string query = "SELECT name, level, class FROM characters WHERE owner = '" + req.get_userid() + "'";
			res = PQexec(conn, query.c_str()); 
			if (PQresultStatus(res) != PGRES_TUPLES_OK) {
				fprintf(stderr, "Fetching characters failed: %s", PQerrorMessage(conn));
				PQclear(res);
				PQfinish(conn);
				exit(1);
			}
			
			if (PQntuples(res) > 0) {
				rep_msg += "You have the following characters:\n";
				for (int i = 0; i < PQntuples(res); i++) {
					// If I have to use this sort of thing a lot and it becomes cumbersome, I'll just use stringstreams instead
					std::string charname(PQgetvalue(res, i, 0));
					std::string charlevel(PQgetvalue(res, i, 1));
					std::string charclass(PQgetvalue(res, i, 2));

					rep_msg += charname + ", level " + charlevel + " " + charclass + "\n";
				}

				rep_msg += "\nType `play CHARACTERNAME` in order to select a character, or `create` to create a new one.\n";
			}

			else {
				rep_msg += "You have no characters yet. To create a character, type `create`.\n";
			}

			rep.set(rep_msg);
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
