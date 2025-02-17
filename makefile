# CC = gcc
# CFLAGS = -Wall -Wextra
# LDFLAGS = -lm
# TARGET = huff_dec
# SRC = huff_dec.c
# BIN_DIR = bin

# all: $(BIN_DIR)/$(TARGET)

# $(BIN_DIR)/$(TARGET): $(SRC)
# 	mkdir -p $(BIN_DIR)
# 	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(TARGET) $(SRC) $(LDFLAGS)

# clean:
# 	rm -f $(BIN_DIR)/$(TARGET)
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm
TARGET = huff_dec
SRC = huff_dec.c
BIN_DIR = bin

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(SRC)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(TARGET) $(SRC) $(LDFLAGS)

run: $(BIN_DIR)/$(TARGET)
	gdb --args $(BIN_DIR)/$(TARGET)

valgrind: $(BIN_DIR)/$(TARGET)
	valgrind --leak-check=full --track-origins=yes $(BIN_DIR)/$(TARGET)

clean:
	rm -f $(BIN_DIR)/$(TARGET)
