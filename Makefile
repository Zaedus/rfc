CC = gcc
BIN := rfc

MAIN_FILE := main

CFLAGS := $(shell curl-config --cflags) 
LDFLAGS := $(shell curl-config --libs) -lm


all: $(BIN)

clean:
	-rm $(MAIN_FILE).o

$(BIN): $(MAIN_FILE).o
	$(CC) -o $(BIN) $(MAIN_FILE).o $(LDFLAGS)

main.o: main.c
	$(CC) -c $(MAIN_FILE).c $(CFLAGS)

install: $(BIN)
	install -Dm755 $(BIN) /usr/bin/$(BIN)