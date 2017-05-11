CC = g++
OBJ = sorcery.o Request.o Response.o PlayerList.o NewCharacterList.o CharacterList.o Server.o

#NOTE: move libpq requirement to Server class instead of main once database stuff is implemented
#$(CC) -std=gnu++11 -o sorcery.o -c main.cpp -I/usr/include/postgresql -L/usr/lib/x86_64-linux-gnu -lpq -lzmq -Wall

game_server: $(OBJ)
	$(CC) -o game_server $(OBJ) -lpq -lzmq -Wall

sorcery.o: main.cpp
	$(CC) -std=gnu++11 -o sorcery.o -c main.cpp -I/usr/include/postgresql -lzmq -Wall

Request.o: Request.cpp
	$(CC) -std=gnu++11 -o Request.o -c Request.cpp -lzmq -Wall

Response.o: Response.cpp
	$(CC) -std=gnu++11 -o Response.o -c Response.cpp -lzmq -Wall

PlayerList.o: PlayerList.cpp
	$(CC) -std=gnu++11 -o PlayerList.o -c PlayerList.cpp -Wall

NewCharacterList.o: NewCharacterList.cpp
	$(CC) -std=gnu++11 -o NewCharacterList.o -c NewCharacterList.cpp -Wall

CharacterList.o: CharacterList.cpp
	$(CC) -std=gnu++11 -o CharacterList.o -c CharacterList.cpp -Wall

Server.o: Server.cpp
	$(CC) -std=gnu++11 -o Server.o -c Server.cpp -I/usr/include/postgresql -Wall
	
clean:
	rm -f game_server $(OBJ)
