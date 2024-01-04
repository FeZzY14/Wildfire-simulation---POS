CC = g++
CFLAGS = -std=c++11 -Wall

all: server klient

server: Server.o ServerMain.o
	${CC} ${CFLAGS} ServerMain.o Server.o -o server -lpthread

klient: client.o Bunka.o Bunka.o Generator.o Generator.o main.o PoziarnyBiotop.o Svet.o Svet.o Vietor.o
	${CC} ${CFLAGS} client.o Bunka.o Bunka.o Generator.o Generator.o main.o PoziarnyBiotop.o Svet.o Svet.o Vietor.o -o klient -lpthread

main.o: main.cpp
	${CC} ${CFLAGS} -c main.cpp

svet.o: svet.cpp svet.h
	g++ -c Svet.cpp

clean:
	rm *.o server klient