CC=gcc
EM=emcc
# for static linking with raylib LIBS=-lraylib -L$(RLPATH) -I$(RLPATH) -lGL -lm -lpthread -ldl -lrt -lX11
RLPATH=./raylib/src

LIBS=-lraylib
CFLAGS=-Wall -Wextra -std=c23 -pedantic -ggdb -O0

EMFLAGS=-Os -Wall -I. -I./raylib/src -L. -L./raylib/src/libraylib.a -s USE_GLFW=3 -s ASYNCIFY -DPLATFORM_WEB --shell-file ./raylib/src/minshell.html

web: main.c $(RLPATH)/libraylib.a
	$(EM) $(CFLAGS) $(EMFLAGS) $^ -o game.html


native: main.c
	$(CC) $(CFLAGS) $(LIBS) $^ -o out
