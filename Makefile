CC=gcc
MAIN=./main
CFLAGS=-std=gnu99 -Wall -g
DEBUG_FLAGS=-fsanitize=address

ifdef DEBUG
    CFLAGS += $(DEBUG_FLAGS)
endif

build: main.c
	$(CC) $(CFLAGS) main.c -o $(MAIN)

run: build
	./main

clean:
	rm -f $(MAIN)
