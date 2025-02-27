CC = gcc
CFLAGS = -Wall -g

.DEFAULT_GOAL := help

.PHONY: help
help:
	@echo "Usage:"
	@echo "make exercise1"
	@echo "make exercise2"
	@echo "make exercise3"
	@echo "make exercise4"
	@echo "make matrix"


exercise1: exercise1.c

exercise2: exercise2.c

exercise3: exercise3.c

matrix: matrix.o searchM.o

matrix.o: matrix.c matrix.h

searchM.o: searchM.c matrix.h

clean:
	rm -f *.o
	rm -f exercise1
	rm -f exercise2
	rm -f exercise3
	rm -f exercise4
	rm -f matrix
