all:
	g++ -o server server.cpp Request.cpp Response.cpp -I/usr/include/postgresql -L/usr/lib/x86_64-linux-gnu -lpq -lzmq -Wall

