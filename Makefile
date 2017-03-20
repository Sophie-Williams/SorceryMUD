all:
	g++ -o server server.cpp Request.cpp Response.cpp -lzmq -Wall

