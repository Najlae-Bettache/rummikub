CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2
OUT=rummikub

SRC=src/main.c src/data.c src/game.c src/debug.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
