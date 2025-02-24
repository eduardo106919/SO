CC = gcc
CFLAGS = -Wall -g

.DEFAULT_GOAL := help

.PHONY: help
help:
	@echo "Usage:"
	@echo "make exercise1"
	@echo "make exercise2"
	@echo "make exercise3"
# @echo "make exercise4"
# @echo "make matrix"


exercise1: exercise1.o
	$(CC) $(CFLAGS) $< -o $@

exercise2: exercise2.o
	$(CC) $(CFLAGS) $< -o $@

exercise3: exercise3.o
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm -f *.o
	rm -f exercise1
	rm -f exercise2
	rm -f exercise3
