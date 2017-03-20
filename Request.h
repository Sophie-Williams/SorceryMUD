// Request class for SorceryMUD, contains all the information of one message sent in Discord

#ifndef REQUEST_H 
#define REQUEST_H 

#include <zmq.hpp>
#include <string>

class Request {
	private:
		std::string userid;
		std::string content;

	public:
		Request(zmq::socket_t&);
		//~Request();

		char* append_null(char*, int);
		std::string get_userid() { return userid; }
		std::string get_content() { return content; }
};

#endif

