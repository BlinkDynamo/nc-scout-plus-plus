CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/nc-scout.c src/validation.c src/config.c src/naming.c
OBJ = $(SRC:src/%.c=build/%.o)
TARGET = build/nc-scout

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build

test: all
	@echo "Running tests (to be implemented)..."

.PHONY: all clean test

