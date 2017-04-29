#include "Request.h"
#include "Response.h"
#include "Playerlist.h"
#include "NewCharacterList.h"

#include <string>
#include <iostream>
#include <locale>
#include <zmq.hpp>
#include <libpq-fe.h>
#include "json.hpp"
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>

#define sleep(n)    Sleep(n)
#endif

int main() {
	std::cout << "SorceryMUD, a MUD game created to demonstrate the potential of MUDBot" << std::endl;

	//std::locale loc;
    zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	std::cout << "Binding socket to localhost:5555 ... " << std::endl;
    socket.bind("tcp://*:5555");
	std::cout << "Listening for requests" << std::endl;

	Playerlist connected; // Holds info of all the players who are connected to the game server
	NewCharacterList new_chars; // Holds info of characters in the process of being created (max 1 per player)
	
	PGresult *res;
	PGconn *conn = PQconnectdb("host=localhost dbname=sorcery user=sorcery password=ryu5g7cwq89t97z5t4yq");
	if (PQstatus(conn) != CONNECTION_OK) {
		fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
		PQfinish(conn);
		exit(1);
	}

    while (true) {
		Request req(socket);
		Response rep;

		std::string content = req.get_content();
		std::string userid = req.get_userid();

		std::cout << "Received message from user " << userid << ": '" << content << "'" << std::endl;

		//  Process the message based on player state
		int state = connected.playerstate(userid);
	
		if (state == 0) { // If user is not connected
			if (content == "connect") {
				// It's impossible to connect through the Discord bot while already connected.
				// However, there is a possibility that someone could send messages through other means and bypass the restrictions placed by the client.
				// The user should only be allowed to connect if their ID is not in connected.
				if (connected.add(userid, 10) == -1) {
					printf("Failed to add user: already connected");
				}
				// Retrieve character data for the user, and display it
				// Provide options to select, create, or delete a character
				std::string rep_msg = "Welcome to SorceryMUD.\n\n"; // Insert ASCII art here

				// Create a new row for the player if and only if they do not already have one
				std::string player_query = "INSERT INTO players (userid, first_activity, last_activity, state) SELECT '" + userid + "', current_date, current_date, 0 WHERE NOT EXISTS (SELECT userid FROM players WHERE userid = '" + userid + "')";
				res = PQexec(conn, player_query.c_str());
				if (PQresultStatus(res) != PGRES_COMMAND_OK) {
					fprintf(stderr, "Fetching characters failed: %s", PQerrorMessage(conn));
					PQclear(res);
					PQfinish(conn);
					exit(1);
				}
	
				std::string chars_query = "SELECT name, level, class, complete FROM characters WHERE owner = '" + userid + "'";
				res = PQexec(conn, chars_query.c_str()); 
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
						std::string charcomplete(PQgetvalue(res, i, 3));
	
						rep_msg += "\t* " + charname; 
	
						if (charcomplete == "t") {
							rep_msg += ", Level " + charlevel + " " + charclass;
						}
	
						else {
							rep_msg += " [INCOMPLETE]";
						}
	
						rep_msg += "\n";
					}
	
					rep_msg += "\nType the name of a character in order to select them, or `create` to create a new character.";
				}
	
				else {
					rep_msg += "You have no characters yet. To create a character, type `create`.";
				}
	
				rep.set(rep_msg);
			}
		}
	
		else { // If user is connected
			if (content == "disconnect") {
				// Check if player is connected
				// Save player data to the database, set state to 10
				// Remove player from connected list
				rep.set(""); // "Dummy" response to preserve req-rep pattern
			}
	
			else if (state == 10) { // If user is connected and in state 10 (menu)
				if (content == "create") {
					rep.set("Enter a name for the new character. Note that character names have a maximum length of 12 characters and cannot contain special (non-alphanumeric) characters.");
					connected.setplayerstate(userid, 20);
				}

				else {
					// Retrieve the player's character list and iterate through them, checking against req.get_content()
					std::string chars_query = "SELECT name, charid FROM characters WHERE owner = '" + userid + "' AND complete"; 
					res = PQexec(conn, chars_query.c_str());
					if (PQresultStatus(res) != PGRES_TUPLES_OK) {
						fprintf(stderr, "Fetching characters failed: %s", PQerrorMessage(conn));
						PQclear(res);
						PQfinish(conn);
						exit(1);
					}

					std::string charid; // Even though charid is stored as an int in the DB, it's convenient to have a string here
					for (int i = 0; i < PQntuples(res); i++) {
						std::string charname(PQgetvalue(res, i, 0));
						if (content == charname) { // Should be case-insensitive
							charid = PQgetvalue(res, i, 1);
							break;
						}
					}

					if (!charid.empty()) {
						// Select the character for playing, etc.
						// Change the user's state
						rep.set("Nice job, you selected a character correctly. Except, I don't have anything here, so, nothing happens. Functionality will be added soon ... ");
					}					

					else {
						rep.set("No character with that name was found. Type the name of a character in order to select them, or `create` to create a new character.");
					}
				}
			}

			else if (state == 20) { // Naming
				// Enforce naming restrictions here
				if (content.length() > 12) {
					rep.set("Character names have a maximum length of 12 characters. Please enter a different name.");
				}

				else if (content == "create" || content == "quit" || content == "disconnect") { // If this list grows much longer I'll have to do it another way
					rep.set("That name is not allowed. Please enter a different name.");
				}

				else {
					// Check for non-alphanumeric characters
					bool alnum = true;
					for (int i = 0; i < content[i]; i++) {
						if (!std::isalnum(content[i])) {
							alnum = false;
							break;
						}
					}

					if (!alnum) {
						rep.set("Character names may not contain special (non-alphanumeric) characters. Please enter a different name.");
					}

					else {
						// Retrieve character list and verify the new name is not the same
						std::string chars_query = "SELECT name FROM characters WHERE owner = '" + userid + "'";
						res = PQexec(conn, chars_query.c_str());
						if (PQresultStatus(res) != PGRES_TUPLES_OK) {
							fprintf(stderr, "Fetching characters failed: %s", PQerrorMessage(conn));
							PQclear(res);
							PQfinish(conn);
							exit(1);
						}

						bool unique = true;
						for (int i = 0; i < PQntuples(res); i++) {
							std::string charname(PQgetvalue(res, i, 0));
							if (content == charname) { // This should be case-insensitive
								unique = false;
								break;
							}
						}

						if (!unique) {
							rep.set("You already have a character with that name. Please enter a different name.");
						}

						else {
							// The name is valid
							new_chars.add(userid);
							new_chars.set_name(userid, content);
							rep.set("Enter the character's gender. You may either type `m` or `f`.");
							connected.setplayerstate(userid, 21);
						}
					}
				}
			}

			else if (state == 21) {
				if (content == "m" || content == "f") {
					new_chars.set_name(userid, content);
					rep.set("Race Selection (I haven't put any thought into the actual races in this game so here are some placeholders)\n\n\t* Crab\n\t* Tortoise\n\t* Hare");
					connected.setplayerstate(userid, 22);
				}

				else {
					rep.set("That response is not valid. Please only either type `m` or `f`.");
				}
			}

			else if (state == 22) {
				rep.set("You chose THAT? Wow, what a ... er, interesting choice.");
			}

			else if (state == 23) {

			}

			else {
				rep.set("You have been pwned by a grue.");
			}
		}

		rep.send(socket);

		sleep(1); // Wait before running the loop again. In the future, this should be changed
	}

	return 0;
}
