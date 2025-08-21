all: client server

client: client.c common.h
	gcc client.c -Wall -Wextra -o client -g

server: server.c common.h
	gcc server.c -Wall -Wextra -o server -g -lpthread -lrt

.PHONY: clear
clear:
	rm client
	rm server
