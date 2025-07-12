cc = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/main.c src/cache.c src/utils.c
OUT = zerostore

all:
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)