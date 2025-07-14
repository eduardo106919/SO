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
	@echo "make exercise5"
	@echo "make exercise6"
	@echo "make exercise7"

exercise1: exercise1.c

exercise2: exercise2.c

exercise3: exercise3.c

exercise4: exercise4.c

exercise5: exercise5.c matrix.o matrix.h

exercise6: exercise6.c matrix.o matrix.h

exercise7: exercise7.c matrix.o matrix.h

matrix.o: matrix.c matrix.h

clean:
	rm -f *.o
	rm -f exercise1
	rm -f exercise2
	rm -f exercise3
	rm -f exercise4
	rm -f exercise5
	rm -f exercise6
	rm -f exercise7
	rm -f matrix
