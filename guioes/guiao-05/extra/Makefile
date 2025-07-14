CC = gcc
CFLAGS = -Wall -g


all: server client

server: server.o vector.o

client: client.o

server.o: server.c defs.h vector.h

client.o: client.c

vector.o: vector.c defs.h vector.h

clean:
	rm -f *.out
	rm -f *.o
	rm -f *.log
	rm -f fifo_*
	rm -f server
	rm -f client