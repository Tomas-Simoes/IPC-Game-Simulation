CC=gcc
CFLAGS=-Wall -Iinclude

all: player1 player2

player1: src/player1.c src/game.c src/ipc.c
	$(CC) $(CFLAGS) -o player1 src/player1.c src/game.c src/ipc.c

player2: src/player2.c src/game.c src/ipc.c
	$(CC) $(CFLAGS) -o player2 src/player2.c src/game.c src/ipc.c

clean:
	rm -f player1 player2
