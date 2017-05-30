CC = g++
OBJ = sorcery.o Request.o Response.o PlayerList.o NewCharList.o PlayerCharList.o RoomList.o Server.o

#NOTE: move libpq requirement to Server class instead of main once database stuff is implemented
#$(CC) -std=gnu++11 -o sorcery.o -c main.cpp -I/usr/include/postgresql -L/usr/lib/x86_64-linux-gnu -lpq -lzmq -Wall

game_server: $(OBJ)
	$(CC) -o sorcery $(OBJ) -lpq -lzmq -Wall

sorcery.o: main.cpp
	$(CC) -std=gnu++11 -o sorcery.o -c main.cpp -I/usr/include/postgresql -lzmq -Wall

Request.o: Request.cpp
	$(CC) -std=gnu++11 -o Request.o -c Request.cpp -lzmq -Wall

Response.o: Response.cpp
	$(CC) -std=gnu++11 -o Response.o -c Response.cpp -lzmq -Wall

PlayerList.o: PlayerList.cpp
	$(CC) -std=gnu++11 -o PlayerList.o -c PlayerList.cpp -Wall

NewCharList.o: NewCharList.cpp
	$(CC) -std=gnu++11 -o NewCharList.o -c NewCharList.cpp -Wall

PlayerCharList.o: PlayerCharList.cpp
	$(CC) -std=gnu++11 -o PlayerCharList.o -c PlayerCharList.cpp -Wall

RoomList.o: RoomList.cpp
	$(CC) -std=gnu++11 -o RoomList.o -c RoomList.cpp -Wall

Server.o: Server.cpp
	$(CC) -std=gnu++11 -o Server.o -c Server.cpp -I/usr/include/postgresql -Wall
	
clean:
	rm -f game_server $(OBJ)
