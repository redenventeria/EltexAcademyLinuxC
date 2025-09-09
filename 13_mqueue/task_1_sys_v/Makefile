all: client server

client: client.c
	gcc client.c -Wall -Wextra -o client -g -lrt

server: server.c
	gcc server.c -Wall -Wextra -o server -g -lrt

PHONY.:clean

clean:
	rm client
	rm server
	rm queue
