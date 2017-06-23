#include "Server.h"

Server::Server(zmq::context_t& context)
	: r_socket(context, ZMQ_REP), s_socket(context, ZMQ_PUB) {
	CMD_INVALID = "That is not a valid command.";
}

std::string Server::look(std::string userid) {
	return look_roomid(userid, chars.get_loc(userid));
}

std::string Server::look_roomid(std::string userid, int roomid) {
	Room* r = rooms.get_room(roomid); 
	std::string rep_msg = r->desc + "\n\n[ Exits: ";

	for (auto it = r->exits.begin(); it != r->exits.end(); ++it) {
		rep_msg += it->name + " ";
	}

	rep_msg += "]";

	// These loops could probably be done in a different way, but that's not important right now

	// Show all players in the room
	// This gets a bit unwieldy because I have to make sure the player doesn't see himself/herself
	int i, player_amnt = r->players.size();
	for (i = 0; i < player_amnt; i++) {
		PlayerChar character = r->players[i];
		if (character.owner == userid) {
			i++;
			break;
		}
		
		rep_msg += "\n" + character.name + " is here.";
	}

	for (; i < player_amnt; i++) { // Loop twice for a tiny optimization
		PlayerChar character = r->players[i];
		rep_msg += "\n" + character.name + " is here.";
	}

	// Show all NPCs in the rooms
	int j, npc_amnt = r->npcs.size();
	for (j = 0; j < npc_amnt; j++) {
		NonPlayerChar npc = r->npcs[j];
		rep_msg += "\n" + npc.desc;
	}

	return rep_msg;
}

std::string Server::in_game(std::string userid, std::string content) {
	//std::vector<std::string> input;
	std::istringstream ss(content);
	/*while (!ss.eof()) {
		std::string word;
		std::getline(ss, word, ' ');
		input.push_back(word);
	}*/

	std::string cmd, args;
	std::getline(ss, cmd, ' ');
	std::getline(ss, args);

	if (cmd == "look") {
		if (args == "") {
			// Show the room
			return look(userid);
		}

		// Show a specific thing
		try {
			return rooms.get_room(chars.get_loc(userid))->get_lookable(args).look_string;
		}

		catch (lookable_not_found) {
			return "I don't see '" + args + "' here.";
		}
	}

	if (cmd == "go") {
		if (args == "") {
			return "Go where?";
		}

		int loc = chars.get_loc(userid);
		int exit_amnt = rooms.exit_amnt(loc);
		for (int p = 0; p < exit_amnt; p++) {
			Exit exit = rooms.get_exit(loc, p);
			if (exit.name == args) {
				// Change character's location, execute look command 
				move_char(userid, loc, exit.dest);
				return look_roomid(userid, exit.dest);
			}
		}

		return "I don't know where '" + args + "' is.";
	}

	return "I don't know what '" + cmd + "' is.";
}

std::string Server::main_menu(std::string userid, std::string content) {
	// Retrieve character data
	std::string chars_query = "SELECT name, level, class, complete FROM characters WHERE owner = '" + userid + "'";
	PGresult* player_chars = dbselect(chars_query);
	std::string rep_msg;

	// Display the characters
	if (PQntuples(player_chars) > 0) {
		rep_msg += "You have the following characters:\n\n";
		for (int i = 0; i < PQntuples(player_chars); i++) {
			std::string charname(PQgetvalue(player_chars, i, 0));
			std::string charlevel(PQgetvalue(player_chars, i, 1));
			std::string charclass(PQgetvalue(player_chars, i, 2));
			std::string charcomplete(PQgetvalue(player_chars, i, 3));

			rep_msg += charname;

			if (charcomplete == "t") {
				rep_msg += ", Level " + charlevel + " " + charclass;
			}

			else {
				rep_msg += " [INCOMPLETE]";
			}

			rep_msg += "\n"; 
		}

		rep_msg += "\nType the name of a character in order to select them, or `create` to create a new character.";
		return rep_msg;
	}

	rep_msg += "You have no characters yet. To create a character, type `create`.";
	return rep_msg;
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
		// Create a new row for the player if and only if they do not already have one
		std::string player_query = "INSERT INTO players (userid, first_activity, last_activity, state) SELECT '" + userid + "', current_date, current_date, 0 WHERE NOT EXISTS (SELECT userid FROM players WHERE userid = '" + userid + "')";
		dbcommand(player_query);
		return "Welcome to SorceryMUD.\n\n" + main_menu(userid, content); // Insert ASCII art here
	}		

	return CMD_INVALID;
}

std::string Server::menu(std::string userid, std::string content) {
	if (content == "create") {
		connected.setplayerstate(userid, 20);
		return "Enter a name for the new character. Note that character names have a maximum length of 12 characters and cannot contain special (non-alphanumeric) characters.";
	}
	
	// Retrieve the player's character list and iterate through them, checking against content
	std::string chars_query = "SELECT name FROM characters WHERE owner = '" + userid + "' AND complete";
	PGresult* player_chars = dbselect(chars_query);

	for (int i = 0; i < PQntuples(player_chars); i++) {
		std::string charname = PQgetvalue(player_chars, i, 0);
		if (content == charname) { // Should be case-insensitive
			// Select the character and change the user's state
			connected.setplayerstate(userid, 40);
			if (chars.char_exists_with_user(userid)) {
				return "You are already playing as another character.";
			}

			load_char(charname, userid);
			std::string rep_msg = look(userid);
			return rep_msg;
		}
	}

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
	std::string chars_query = "SELECT name FROM characters WHERE owner = '" + userid + "'";
	PGresult *player_chars = dbselect(chars_query);
	
	for (int i = 0; i < PQntuples(player_chars); i++) {
		std::string charname(PQgetvalue(player_chars, i, 0));
		if (charname == content) { // Should be case insensitive
			return "You already have a character with that name. Please enter a different name."; 
		}
	}

	// The name is valid
	newchars.add(userid);
	newchars.set_name(userid, content);
	connected.setplayerstate(userid, 21);
	return "Enter the character's gender. You may either type `m` or `f`.";
}

std::string Server::select_gender(std::string userid, std::string content) {
	if (content == "m" || content == "f") {
		newchars.set_gender(userid, content); 
		connected.setplayerstate(userid, 22);
		return "Select the character's race. (I haven't put any thought into the actual races in this game so here are some placeholders.)\n\nCrab - _Jack of few trades and master of none, the crab is an excellent race to play if you like being disappointed._\n\nTortoise - _Tortoises aspire to nothing less than greatness; unfortunately, they rarely achieve anything more than mediocrity. You would be better off playing as a sea slug._\n\nHare - _Though cute, hares offer nothing else of value and face a severe overpopulation problem in this universe anyway._";
	}

	return "That response is not valid. Please only either type `m` or `f`.";
}

std::string Server::select_race(std::string userid, std::string content) {
	if (content == "crab" || content == "tortoise" || content == "hare") {
		newchars.set_race(userid, content);
		connected.setplayerstate(userid, 23);
		return "Select the character's class. (Placeholder classes ahoy.)\n\nDragonslayer - _Dragonslayers have one purpose: to defend their homes and people against dragonkind. Unfortunately, dragons are entirely fictional beings, and as such, dragonslayers typically must take a second job in order to put food on the table._\n\nSorcerer - _Sorcerers claim to be able to harness the dark energies of the universe and shape the world to their will, and as such are formidable enemies if you are very superstitious. However, no evidence has yet been presented to corroborate this claim._\n\nThief - _Thieves specialize in the art of stealth and, well, thievery, of course. They are experts at all things theft-related, from credit card fraud to embezzlement to larceny. Most thieves are arrested only after realizing that crouching doesn't make them invisible._\n\nHealer - _Healers are noble and compassionate individuals who dedicate their lives toward helping others, or are just in it for the money. The path to becoming a healer is strenuous and difficult. After four years of intense training, students must undergo a time-honored test to prove themselves worthy of further study, usually referred to as the \"MCAT\"._\n\nDruid - _Look, sakuras!_";
	}

	return "That is not a race. Please choose from the races provided.";
}

std::string Server::select_class(std::string userid, std::string content) {
	if (content == "dragonslayer" || content == "sorcerer" || content == "thief" || content == "healer" || content == "druid") {
		newchars.set_class(userid, content);

		connected.setplayerstate(userid, 24);
		std::string rep_msg = "Please review the following details and make sure they are correct.\n\nName: " + newchars.get_name(userid) + "\nGender: ";
		if (newchars.get_gender(userid) == "m") {
			rep_msg += "Male\n";
		}

		else {
			rep_msg += "Female\n";
		}

		// There is no need to capitalize the first letter of the race and class here, since that is already done in the set_race function
		rep_msg += "Race: " + newchars.get_race(userid) + "\nClass: " + newchars.get_class(userid) + "\n\nTo confirm your choices, type `ok`. Otherwise, type `back` to go back or `restart` to start the character creation process over.";
		return rep_msg;
	}

	return "That is not a class. Please choose from the classes provided.";
}

std::string Server::newchar_confirm(std::string userid, std::string content) {
	if (content == "ok") {
		// Add the character to the database
		std::string add_query = "INSERT INTO characters (owner, name, gender, race, class, level, xp, location, complete) VALUES ('" + userid + "', '" + newchars.get_name(userid) + "', '" + newchars.get_gender(userid) + "', '" + newchars.get_race(userid) + "', '" + newchars.get_class(userid) + "', 1, 0, 1, 't')";
		dbcommand(add_query);

		// Return to main menu
		connected.setplayerstate(userid, 10);
		return "Character successfully created. Returning to main menu.\n\n" + main_menu(userid, content);
	}

	if (content == "back" || content == "restart") {
		return "Unfortunately, that feature is not available at this time.";
	}

	return CMD_INVALID;
}

void Server::move_char(std::string userid, int loc, int dest) {
	PlayerChar ch = chars.get_char(userid);
	rooms.remove_player(loc, userid);
	notify_room(loc, ch.name + " has left toward the " + rooms.get_connected_exit(loc, dest).name + ".");
	chars.set_loc(userid, dest);
	notify_room(dest, ch.name + " has arrived from the " + rooms.get_connected_exit(dest, loc).name + ".");
	rooms.add_player(dest, ch);

	for (int i = 0; i < rooms.npc_amnt(dest); i++) {
		std::vector<std::string> dialogue = rooms.get_npc(dest, i).dialogue;
		if (dialogue.size() > 0) {
			// I'm not sure why this is in the correct order but I'll take it
			for (auto it = dialogue.begin(); it != dialogue.end(); ++it) {
				notify_player(userid, *it);
			}
		}
	}

}

void Server::load_char(std::string charname, std::string userid) {
	std::string load_query = "SELECT gender, race, class, level, xp, location, HP_current, HP_max, money FROM characters WHERE name = '" + charname + "' and owner = '" + userid + "'";
	PGresult* chardata = dbselect(load_query);

	if (PQntuples(chardata) == 0) {
		throw "No character with name '" + charname + "'.";
	}

	if (PQntuples(chardata) > 1) {
		throw "Player has more than one character of the same name";
	}

	//std::string race = PQgetvalue(chardata, 0, 1);
	//std::string game_class = PQgetvalue(chardata, 0, 2);
	//int loc = atoi(PQgetvalue(chardata, 0, 5));

	PlayerChar ch;
	ch.name = charname;
	ch.gender = PQgetvalue(chardata, 0,0)[0];
	ch.race = PQgetvalue(chardata, 0, 1);
	ch.game_class = PQgetvalue(chardata, 0, 2);
	ch.level = atoi(PQgetvalue(chardata, 0, 3));
	ch.xp = atoi(PQgetvalue(chardata, 0, 4));
	ch.loc = atoi(PQgetvalue(chardata, 0, 5));
	ch.hp_cur = atoi(PQgetvalue(chardata, 0, 6));
	ch.hp_max = atoi(PQgetvalue(chardata, 0, 7));
	ch.money = atoi(PQgetvalue(chardata, 0, 8));
	ch.owner = userid;
	ch.init_aliases();
	ch.look_string = "Yup, that sure is a player.";
	chars.add(ch);

	rooms.add_player(ch.loc, ch);
}

void Server::init_npcs(std::string filepath) {
	npctypes.init(filepath); // Load NPC types from JSON file
}

void Server::init_rooms(std::string filepath) {
	std::ifstream file(filepath);
	nlohmann::json roomdata;
	file >> roomdata;

	// Change this to use iterators 
	for (size_t i = 0; i < roomdata["rooms"].size(); i++) {
		Room room;
		room.roomid = roomdata["rooms"][i]["roomid"];
		room.desc = roomdata["rooms"][i]["desc"];

		for (size_t j = 0; j < roomdata["rooms"][i]["exits"].size(); j++) {
			Exit exit;
			exit.name = roomdata["rooms"][i]["exits"][j][0];
			exit.dest = roomdata["rooms"][i]["exits"][j][1];
			exit.init_aliases();
			exit.look_string = "Yup, this is an exit.";
			room.exits.push_back(exit);
		}

		for (size_t k = 0; k < roomdata["rooms"][i]["npcs"].size(); k++) {
			std::string internal = roomdata["rooms"][i]["npcs"][k];
			
			/*NonPlayerChar npctype = npctypes.get_npctype(internal);
			NonPlayerChar npc;
			npc.internal = internal;
			npc.name = npctype.name; // "name" attribute of NonPlayerChar corresponds to "display" field in JSON file
			npc.desc = npctype.desc;
			npc.greeting = npctype.greeting;
			npc.init_aliases(); // Do I have a good reason to call init_aliases() for all NPCs, and not just once per NPC type?*/

			NonPlayerChar npc(npctypes.get_npctype(internal)); // This will just do a shallow copy, which I think is probably fine for now because there are no pointer values that could vary between the individual NPC and its NPC type
			
			npcs.add_npc(npc);
			room.add_npc(npc);
		}

		rooms.add_room(room);
	}
} 

void Server::init(std::string r_port, std::string s_port) {
	r_socket.bind("tcp://*:" + r_port);
	s_socket.bind("tcp://*:" + s_port);
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

void Server::send_err() {
	Response rep;
	rep.set("There was a server error in processing the command.");
	rep.send(r_socket);
}

void Server::handle_req(std::ostream& s) {
	Request req(r_socket);
	Response rep;

	std::string userid = req.get_userid();
	std::string content = req.get_content();
	s << "Message from user " << userid << ": '" << content << "'" << std::endl;

	std::transform(content.begin(), content.end(), content.begin(), ::tolower); // Convert string to lowercase for case-insensitive comparisons

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
				// Save player and character data to the database, set state to 10 (why do I need to do that again? It's just set to 10 upon login anyway...why did I make a state column?)
				connected.remove(userid);
				chars.remove(userid);
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
			rep.set(select_class(userid, content));
		}

		else if (state == 24) {
			rep.set(newchar_confirm(userid, content));
		}

		else if (state == 40) {
			rep.set(in_game(userid, content));
		}

		else {
			rep.set("Invalid player state. Returning to main menu...");
			connected.setplayerstate(userid, 0);
		}
	}
	
	rep.send(r_socket);
}

void Server::notify_room(int roomid, std::string msg) {
	int player_amnt = rooms.player_amnt(roomid);
	std::vector<std::string> users;	

	for (int i = 0; i < player_amnt; i++) {
		users.push_back(rooms.get_player(roomid, i).owner);
	}

	notify(users, msg);
}

void Server::notify_player(std::string userid, std::string msg) {
	std::cout << "Notifying user " << userid << ": " << msg << std::endl;

	Response notif;
	notif.set(msg);
	notif.send_multipart(s_socket);
	notif.set(userid);
	notif.send(s_socket);
}

void Server::notify(std::vector<std::string>& users, std::string msg) {
	// The notify function sends a multipart message that signals the client to notify the specified users. The first part contains the message to display, and all subsequent parts contain user IDs of the players who should be notified.
	// The socket parameter should be the socket that is being used for the pub/sub pattern.

	int users_amnt = users.size();
	if (users_amnt == 0) {
		return;
	}

	Response notif;
	notif.set(msg);
	notif.send_multipart(s_socket);

	int i;
	for (i = 0; i < users_amnt - 1; i++) {
		notif.set(users[i]);
		notif.send_multipart(s_socket);
	}

	notif.set(users[i]);
	notif.send(s_socket);
}
