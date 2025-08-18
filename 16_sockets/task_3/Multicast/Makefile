all: client server

client: client.c
	gcc client.c -Wall -Wextra -o client -g

server: server.c
	gcc server.c -Wall -Wextra -o server -g

.PHONY: clear
clear:
	rm client
	rm server
