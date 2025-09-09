CLI_SRC    := client_IPC.c list.c common.c client_UI.c
SER_SRC    := list.c common.c
CLI_OBJ    := client_IPC.o list.o common.o client_UI.o
SER_OBJ    := list.o common.o
CCFLAGS    := -Wall -Wextra -g -Wpedantic
CLI_LFLAGS := -lcurses

all: $(CLI_OBJ) client server

$(CLI_OBJ): %.o : %.c
	gcc $(CCFLAGS) -c $^ -o $@

client: $(OBJ) client.c
	gcc $(CCFLAGS) $(CLI_OBJ) client.c -o client $(CLI_LFLAGS)

server: $(OBJ) server.c
	gcc $(CCFLAGS) $(SER_OBJ) server.c -o server


PHONY.: clean


clean:
	rm $(wildcard *.o) client server