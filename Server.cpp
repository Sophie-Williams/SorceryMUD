#include "Server.h"

void Server::bind(std::string port) { 
	context = zmq::context_t(1);
	socket = zmq::socket_t(context, ZMQ_REP);
	s << "Binding socket to localhost:" << port << std::endl;
	socket.bind("tcp://*:" + port);
	s << "Listening for requests on port " << port << std::endl;
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
		
		// Retrieve character data
		
		// Display the characters
		
		rep_msg += "enter create or cahracter name\n";
		
		return rep_msg;
	}
}

std::string Server::menu(std::string userid, std::string content) {
	if (content == "create") {
		connected.setplayerstate(userid, 20);
		return "Enter a name for the new character. Note that character names have a maximum length of 12 characters and cannot contain special (non-alphanumeric) characters.";
	}
	
	// Retrieve the player's character list and iterate through them, checking against content
	
	return "stuff";
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

std::string Server::select_race(std:;string userid, std::string content) {
	return "You chose THAT? Wow, what a ... er, interesting choice.";
}

void Server::handle_req() {
	Request req(socket);
	Request rep;

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
				connected.remove(std::string);
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
