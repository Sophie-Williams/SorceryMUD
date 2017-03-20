// Response class for SorceryMUD, assists in sending messages to the client
// (It's not about processing the input, it's about sending a message)

#ifndef RESPONSE_H
#define RESPONSE_H

#include <zmq.hpp>
#include <string>

class Response {
	private:
		std::string content;

	public:
		Response() { }
		Response(std::string str) { content = str; }
		//~Response();
		
		void set(std::string str) { content = str; }
		void add(std::string str) { content += str; }
		void send(zmq::socket_t&);
};

#endif
