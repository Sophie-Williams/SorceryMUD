CC = g++
OBJ = sorcery.o Request.o Response.o Playerlist.o NewCharacterList.o Server.o

#NOTE: move libpq requirement to Server class instead of main once database stuff is implemented

sorcery: $(OBJ)
	$(CC) -o sorcery $(OBJ) -lpq -lzmq -Wall

sorcery.o: main.cpp
	$(CC) -std=gnu++11 -o sorcery.o -c main.cpp -I/usr/include/postgresql -L/usr/lib/x86_64-linux-gnu -lpq -lzmq -Wall

Request.o: Request.cpp
	$(CC) -std=gnu++11 -o Request.o -c Request.cpp -lzmq -Wall

Response.o: Response.cpp
	$(CC) -std=gnu++11 -o Response.o -c Response.cpp -lzmq -Wall

Playerlist.o: Playerlist.cpp
	$(CC) -std=gnu++11 -o Playerlist.o -c Playerlist.cpp -Wall

NewCharacterList.o: NewCharacterList.cpp
	$(CC) -std=gnu++11 -o NewCharacterList.o -c NewCharacterList.cpp -Wall

Server.o: Server.cpp
	$(CC) -std=gnu++11 -o Server.o -c Server.cpp -Wall
	
clean:
	rm -f sorcery $(OBJ)
