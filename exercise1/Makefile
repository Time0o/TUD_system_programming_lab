SRC=main.c sincos.c
OBJ=$(SRC:.c=.o)
DEPS=deps.d
BIN=sincos

CC=gcc
CFLAGS=-Wall -Werror -std=gnu99

.DEFAULT_GOAL := $(BIN)

depend: $(SRC)
	$(CC) -MM $^ -MF $(DEPS)

$(BIN): $(OBJ) depend
	$(CC) $(CFLAGS) -o $@ $(OBJ) -lm

.PHONY: clean

clean:
	rm -f $(OBJ) $(DEPS) $(BIN)

-include $(DEPS)
