all:
	g++ -o server server.cpp Request.cpp -lzmq -Wall
