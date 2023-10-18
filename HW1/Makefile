# Variables
CC = gcc
CFLAGS = -Wall -g

# Targets
all: memgrind

memgrind: memgrind.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o memgrind memgrind.c mymalloc.c

clean:
	rm memgrind