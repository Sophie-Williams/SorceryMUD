CC = g++
OBJ = sorcery.o Request.o Response.o PlayerList.o NewCharList.o PlayerCharList.o NonPlayerCharList.o Room.o RoomList.o Server.o
FLAGS = -std=gnu++11 -Wall
#NOTE: move libpq requirement to Server class instead of main once database stuff is implemented
#$(CC) $(FLAGS) -o sorcery.o -c main.cpp -I/usr/include/postgresql -L/usr/lib/x86_64-linux-gnu -lpq -lzmq 

game_server: $(OBJ)
	$(CC) $(FLAGS) -o sorcery $(OBJ) -lpq -lzmq

sorcery.o: main.cpp
	$(CC) $(FLAGS) -o sorcery.o -c main.cpp -I/usr/include/postgresql -lzmq

Request.o: Request.cpp
	$(CC) $(FLAGS) -o Request.o -c Request.cpp -lzmq

Response.o: Response.cpp
	$(CC) $(FLAGS) -o Response.o -c Response.cpp -lzmq

PlayerList.o: PlayerList.cpp
	$(CC) $(FLAGS) -o PlayerList.o -c PlayerList.cpp

NewCharList.o: NewCharList.cpp
	$(CC) $(FLAGS) -o NewCharList.o -c NewCharList.cpp

PlayerCharList.o: PlayerCharList.cpp
	$(CC) $(FLAGS) -o PlayerCharList.o -c PlayerCharList.cpp

NonPlayerCharList.o: NonPlayerCharList.cpp
	$(CC) $(FLAGS) -o NonPlayerCharList.o -c NonPlayerCharList.cpp

Room.o: Room.cpp
	$(CC) $(FLAGS) -o Room.o -c Room.cpp

RoomList.o: RoomList.cpp
	$(CC) $(FLAGS) -o RoomList.o -c RoomList.cpp

Server.o: Server.cpp
	$(CC) $(FLAGS) -o Server.o -c Server.cpp -I/usr/include/postgresql
	
clean:
	rm -f sorcery $(OBJ)
