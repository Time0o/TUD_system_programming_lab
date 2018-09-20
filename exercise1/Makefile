SRC=main.c sincos.c
OBJ=$(SRC:.c=.o)
DEPS=deps.d
BIN=sincos

CC=gcc
CFLAGS=-Wall -Werror -std=gnu99

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

.PHONY: clean

clean:
	rm -f $(OBJ) $(DEPS) $(BIN)

depend: $(SRC)
	$(CC) -MM $^ -MF $(DEPS)

-include $(DEPS)
