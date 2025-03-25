CC = gcc
CFLAGS = -Wall -g

.DEFAULT_GOAL := help

.PHONY: help
help:
	@echo "Usage:"
	@echo "make exercise1"
	@echo "make exercise2"

exercise1:
	$(CC) exercise1-a.c -o a.out
	$(CC) exercise1-b.c -o b.out
	$(CC) exercise1-c.c -o c.out

exercise2: server client

server: server.o vector.o

client: client.o

server.o: server.c defs.h vector.h

client.o: client.c

vector.o: vector.c defs.h vector.h


clean:
	rm -f *.o
	rm -f *.out
	rm -f server
	rm -f client
	rm -f fifo_*