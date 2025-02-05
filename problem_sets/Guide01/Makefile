CC = gcc
CFLAGS = -Wall -g

.DEFAULT_GOAL := help

help:
	@echo "Usage:"
	@echo "make mycat"
	@echo "make mycp"
	@echo "make pessoas"
	@echo "make clean"

mycat: mycat.o

mycat.o: mycat.c

pessoas: pessoas.o person.o

person.o: person.c person.h

pessoas.o: pessoas.c person.h

clean:
	rm -f *.o
	rm -f mycat
	rm -f mycp
	rm -f pessoas file_pessoas
