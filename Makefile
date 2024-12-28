CC = clang
CFLAGS = -Wall -Wextra -g

SRC_DIR = src
BUILD_DIR = build

TARGET = $(BUILD_DIR)/hash_table
SOURCE = $(SRC_DIR)/hash_table.c
HEADERS = $(SRC_DIR)/hash_table.h

$(TARGET): $(SOURCE) $(HEADERS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

leaks: $(TARGET)
	leaks --atExit -- ./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: leaks clean