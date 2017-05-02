#include "Server.h"

Server::Server() {
	CMD_INVALID = "That is not a valid command.";
}

std::string Server::not_connected(std::string userid, std::string content) {
	if (content == "connect") {
		// It's impossible to connect through the Discord bot while already connected.
		// However, there is a possibility that someone could send messages through other means and bypass the restrictions placed by the client.
		// The user should only be allowed to connect if their ID is not in connected.
		if (connected.playerstate(userid)) {
			return "You are already connected.";
		}
		
		connected.add(userid, 10);
		std::string rep_msg = "Welcome to SorceryMUD.\n\n"; // Insert ASCII art here
		
		// Create a new row for the player if and only if they do not already have one
		std::string player_query = "INSERT INTO players (userid, first_activity, last_activity, state) SELECT '" + userid + "', current_date, current_date, 0 WHERE NOT EXISTS (SELECT userid FROM players WHERE userid = '" + userid + "')";
		dbcommand(player_query);
		// Retrieve character data
		std::string chars_query = "SELECT name, level, class, complete FROM characters WHERE owner = '" + userid + "'";
		PGresult *chars = dbselect(chars_query);

		// Display the characters
		if (PQntuples(chars) > 0) {
			rep_msg += "You have the following characters:\n";
			for (int i = 0; i < PQntuples(chars); i++) {
				std::string charname(PQgetvalue(chars, i, 0));
				std::string charlevel(PQgetvalue(chars, i, 1));
				std::string charclass(PQgetvalue(chars, i, 2));
				std::string charcomplete(PQgetvalue(chars, i, 3));

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

		return rep_msg;
	}

	return CMD_INVALID;
}

std::string Server::menu(std::string userid, std::string content) {
	if (content == "create") {
		connected.setplayerstate(userid, 20);
		return "Enter a name for the new character. Note that character names have a maximum length of 12 characters and cannot contain special (non-alphanumeric) characters.";
	}
	
	// Retrieve the player's character list and iterate through them, checking against content
	std::string chars_query = "SELECT name, charid FROM characters WHERE owner = '" + userid + "' AND complete";
	PGresult *chars = dbselect(chars_query);

	std::string charid;
	for (int i = 0; i < PQntuples(chars); i++) {
		std::string charname = PQgetvalue(chars, i, 0);
		if (content == charname) { // Should be case-insensitive
			charid = PQgetvalue(chars, i, 1);
			break;
		}
	}

	if (!charid.empty()) { // Why did I make it like this again?? Whe nwill charid ever be an empty string??
		// Select the character for playing, etc.
		// Change the user's state
		return "Selected character"; // Obviously this text will not be here after I implement game stuff
	}

	// Hmm ... actually, I think it was just so I wouldn't need another bool to keep track of, makes sense.
	return "No character with that name was found. Type the name of a character in order to select them, or `create` to create a new character.";
}

std::string Server::naming(std::string userid, std::string content) {
	if (content.length() > 12) {
		return "Character names have a maximum length of 12 characters. Please enter a different name.";
	}

	if (content == "create" || content == "quit" || content == "disconnect") { // If this list grows much longer I'll have to do it another way
		return "That name is not allowed. Please enter a different name.";
	}

	// Check for non-alphanumeric characters
	for (int i = 0; i < content[i]; i++) {
		if (!std::isalnum(content[i])) {
			return "Character names may not contain special (non-alphanumeric) characters. Please enter a different name.";
		}
	}

	// Retrieve character list and verify the new name is not the same

	bool unique = true;
	
	if (!unique) {
		return "You already have a character with that name. Please enter a different name.";
	}

	// The name is valid
	newchars.add(userid);
	newchars.set_name(userid, content);
	connected.setplayerstate(userid, 21);
	return "Enter the character's gender. You may either type `m` or `f`.";
}

std::string Server::select_gender(std::string userid, std::string content) {
	if (content == "m" || content == "f") {
		newchars.set_name(userid, content);
		connected.setplayerstate(userid, 22);
		return "Race Selection (I haven't put any thought into the actual races in this game so here are some placeholders)\n\n\t* Crab\n\t* Tortoise\n\t* Hare";
	}

	return "That response is not valid. Please only either type `m` or `f`.";
}

std::string Server::select_race(std::string userid, std::string content) {
	return "You chose THAT? Wow, what a ... er, interesting choice.";
}

void Server::dbconnect() {
	conn = PQconnectdb("host=localhost dbname=sorcery user=sorcery password=ryu5g7cwq89t97z5t4yq");

	if (PQstatus(conn) != CONNECTION_OK) {
		std::cerr << "Connection to database failed: " << PQerrorMessage(conn);
		PQfinish(conn);
		exit(1);
	}
}

void Server::dbcommand(std::string query) {
	PGresult *res = PQexec(conn, query.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		std::cerr << "Database operation failed: " << PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		exit(1);
	}
}

PGresult* Server::dbselect(std::string query) {
	PGresult *res = PQexec(conn, query.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		std::cerr << "Failed to retrieve information from database: " << PQerrorMessage(conn);
		PQclear(res);
		PQfinish(conn);
		exit(1);
	}

	return res;
}

void Server::handle_req(zmq::socket_t& socket) {
	handle_req(socket, std::cout);
}

void Server::handle_req(zmq::socket_t& socket, std::ostream& s) {
	Request req(socket);
	Response rep;

	std::string userid = req.get_userid();
	std::string content = req.get_content();
	s << "Received message from user " << userid << ": '" << content << "'" << std::endl;

	int state = connected.playerstate(userid);

	if (state == 0) {
		rep.set(not_connected(userid, content));
	}
	
	else {
		if (content == "disconnect") {
			if (connected.playerstate(userid) == 0) {
				rep.set("You are not connected.");
			}

			else {
				// Save player and character data to the database, set state to 10 (why do I need to do that again? It's just set to 10 upon login anyway ... why did I make a state column?)
				connected.remove(userid);
				rep.set(""); // "Dummy" response to preserve req-rep pattern
			}
		}
			
		else if (state == 10) {
			rep.set(menu(userid, content));
		}
		
		else if (state == 20) {
			rep.set(naming(userid, content));
		}
		
		else if (state == 21) {
			rep.set(select_gender(userid, content));
		}

		else if (state == 22) {
			rep.set(select_race(userid, content));
		}

		else if (state == 23) {
			rep.set("how did you even get here");
		}

		else {
			rep.set("You have been pwned by a grue.");
		}
	}
	
	rep.send(socket);
}
