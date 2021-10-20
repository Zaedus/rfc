CC = cc
BIN := rfc

MAIN_FILE := main

CFLAGS := $(shell curl-config --cflags) 
LDFLAGS := $(shell curl-config --libs) -lm


all: $(BIN)

clean:
	-rm $(MAIN_FILE).o

$(BIN): $(MAIN_FILE).o
	cc -o $(BIN) $(MAIN_FILE).o $(LDFLAGS)

main.o: main.c
	cc -c $(MAIN_FILE).c $(CFLAGS)
