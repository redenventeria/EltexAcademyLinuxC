all: client server

client: client.c common.h
	gcc client.c -Wall -Wextra -o client

server: server.c common.h
	gcc server.c -Wall -Wextra -o server

.PHONY: clear
clear:
	rm client
	rm server
