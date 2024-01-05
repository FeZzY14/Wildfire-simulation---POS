CC = g++
CFLAGS = -std=c++11 -Wall

all: server klient

server: Server.o ServerMain.o
	$(CC) $(CFLAGS) ServerMain.o Server.o -o server -lpthread

klient: Client.o Bunka.o main.o Svet.o PoziarnyBiotop.h Vietor.h Generator.h
	$(CC) $(CFLAGS) Client.o Bunka.o main.o Svet.o -o klient -lpthread

main.o: main.cpp PoziarnyBiotop.h Vietor.h Generator.h
	$(CC) $(CFLAGS) -c main.cpp

Bunka.o: Bunka.cpp Bunka.h PoziarnyBiotop.h Vietor.h Generator.h
	$(CC) $(CFLAGS) -c Bunka.cpp

Svet.o: Svet.cpp Svet.h PoziarnyBiotop.h Vietor.h Generator.h
	$(CC) $(CFLAGS) -c Svet.cpp

clean:
	rm *.o server klient
