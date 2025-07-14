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
	@echo "make sosh"

exercise1: exercise1.c

exercise2: exercise2.c

exercise3: exercise3.c

exercise4: exercise4.c auxiliares
	$(CC) $(CFLAGS) $< -o $@

sosh: sosh.c
	$(CC) $(CFLAGS) $< -o $@ -lreadline

auxiliares: auxiliar.c
	$(CC) $(CFLAGS) auxiliar.c -o a.out
	$(CC) $(CFLAGS) auxiliar.c -o b.out
	$(CC) $(CFLAGS) auxiliar.c -o c.out

clean:
	rm -f *.o
	rm -f *.out
	rm -f exercise1
	rm -f exercise2
	rm -f exercise3
	rm -f exercise4
	rm -f sosh
