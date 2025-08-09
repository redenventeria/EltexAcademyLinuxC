BIN := main
OBJ := panel.o interface.o
SRC := panel.c interface.c
HDR := panel.h

CC := gcc
CFLAGS := -g -Wall -Wextra
LDFLAGS := -lncurses

.PHONY: clean

$(BIN): $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -o $(BIN) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $^ -c $(LDFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)

$(BIN): $(HDR)

# $^ - deps
# $@ - target
# make can call other project makes
