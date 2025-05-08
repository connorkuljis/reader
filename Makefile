CC=gcc
MAIN=./main
DEBUG_FLAGS=-fsanitize=address
CFLAGS=-std=gnu99 -Wall -g

ifdef DEBUG
    CFLAGS += $(DEBUG_FLAGS)
endif

build: main.c
	$(CC) $(CFLAGS) main.c -o $(MAIN)

run: build
	./main

clean:
	rm -f $(MAIN)
