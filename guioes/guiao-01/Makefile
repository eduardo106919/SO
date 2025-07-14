CC = gcc
CFLAGS = -Wall -g

.DEFAULT_GOAL := help

help:
	@echo "Usage:"
	@echo "make mycat"
	@echo "make mycp"
	@echo "make people"
	@echo "make clean"

mycat: mycat.o

mycat.o: mycat.c

people: people.o person.o

person.o: person.c person.h

people.o: people.c person.h

clean:
	rm -f *.o
	rm -f mycat
	rm -f mycp
	rm -f people community
