CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lm
TARGET = huff_dec
SRC = huff_dec.c
BIN_DIR = bin

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(SRC)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(BIN_DIR)/$(TARGET)
