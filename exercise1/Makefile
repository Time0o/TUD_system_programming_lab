SRC_DIR=src
INCLUDE_DIR=include
OBJ_DIR=obj
BIN_DIR=bin

_OBJ=main.o sincos.o
OBJ=$(patsubst %.o, $(OBJ_DIR)/%.o, $(_OBJ))

CFLAGS=-Wall -Werror -std=gnu99

$(BIN_DIR)/sincos: $(OBJ)
	gcc $(CFLAGS) -o $@ $^ -lm

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	gcc $(CFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/* $(BIN_DIR)/*
