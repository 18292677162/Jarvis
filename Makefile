DIR=.
INC=./inc
SRC=./src
VPATH=./src

CC=g++
CFLAGS=-Wall -g -I$(INC)

BIN=Jarvis

all:$(BIN)

Jarvis:Jarvis.o
	$(CC) $^ -o $@ -ljsoncpp -lcrypto -lcurl $(CFLAGS)

%.o:%.cc
	$(CC) $(CFLAGS) -c $< -o $@ -std=c++11

clean:
	-rm -rf *.o $(BIN)

.PHONY:all clean
