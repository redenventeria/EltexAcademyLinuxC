all: client server

client: client.c
	gcc client.c -Wall -Wextra -o client

server: server.c
	gcc server.c -Wall -Wextra -o server

.PHONY: clear
clear:
	rm client
	rm server
