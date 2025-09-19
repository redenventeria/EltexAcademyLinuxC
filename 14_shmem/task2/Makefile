CLI_SRC    := client_UI.c
SER_SRC    := 
CLI_OBJ    := client_UI.o
SER_OBJ    := 
CCFLAGS    := -Wall -Wextra -g -Wpedantic
LFLAGS := -lcurses -lpthread -lrt

all: $(CLI_OBJ) client server

$(CLI_OBJ): %.o : %.c
	gcc $(CCFLAGS) -c $^ -o $@

#$(SER_OBJ): %.o : %.c
#	gcc $(CCFLAGS) -c $^ -o $@

client: $(OBJ) client.c
	gcc $(CCFLAGS) $(CLI_OBJ) client.c -o client $(LFLAGS)

server: $(OBJ) server.c
	gcc $(CCFLAGS) $(SER_OBJ) server.c -o server $(LFLAGS)


PHONY.: clean


clean:
	rm $(wildcard *.o) client server