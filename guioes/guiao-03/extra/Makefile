CC = gcc
CFLAGS = -Wall -g

.DEFAULT_GOAL := sosh

sosh: sosh.c
	$(CC) $(CFLAGS) $< -o $@ -lreadline

clean:
	rm -f *.o
	rm -f sosh
