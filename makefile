SOURCE = ./src/backup.c
BIN = ./bin/backupd
CC = cc
FLAGS = -Wall -O2

all:
	$(CC) $(FLAGS) $(SOURCE) -o $(BIN)

clean:
	rm ./bin/*