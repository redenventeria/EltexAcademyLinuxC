HDR := common.h
TARGETS := client server
CC = gcc
CCFLAGS = -Wall -Wextra -Wpedantic -g


all: server client $(HDR)

client: client.c
	$(CC) $(CCFLAGS) -g $? -o $@

server: server.c
	$(CC) $(CCFLAGS) -Wall -Wextra -Wpedantic -g $? -o $@

clear:
	rm $(TARGETS) a.out


