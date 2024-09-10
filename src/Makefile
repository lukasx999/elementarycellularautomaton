
CC=gcc
LIBS=-lraylib
CFLAGS=-Wall -Wextra -std=c23 -pedantic -ggdb -O0 $(LIBS)

all: main.c
	$(CC) $(CFLAGS) $^ -o out
	./out
