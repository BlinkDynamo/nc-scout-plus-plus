#----------------------------------------------------------------------------------------------#
# Variables
#----------------------------------------------------------------------------------------------#
CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -Iinclude -g

# Installation prefix.
PREFIX ?= /usr/local

# Important directories.
BUILD_DIR = build
TESTS_DIR = $(BUILD_DIR)/tests
SRC_DIR = src
BIN_DIR = $(PREFIX)/bin

# Important files.
SRCS = src/main.cpp src/validate.cpp src/naming.cpp src/search.cpp	
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
EXEC = build/nc-scout++
TESTS_EXEC_BUILD = tests/build.sh
TESTS_EXEC_CHECK = tests/check.sh

# Exports. Used by scripts in tests/.
export BUILD_DIR := $(BUILD_DIR)
export TESTS_DIR := $(TESTS_DIR)

#----------------------------------------------------------------------------------------------#
# Dependency Tree 
#----------------------------------------------------------------------------------------------#
all: $(BUILD_DIR) $(EXEC)

check: $(BUILD_DIR) $(TESTS_DIR) $(EXEC)
	$(TESTS_EXEC_BUILD)	
	$(TESTS_EXEC_CHECK)

install: $(EXEC)
	install -d $(DESTDIR)$(BIN_DIR)
	install -m 755 $(EXEC) $(DESTDIR)$(BIN_DIR)

uninstall:
	rm -f $(DESTDIR)$(BIN_DIR)/$(notdir $(EXEC))

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

$(BUILD_DIR)/$(SRC_DIR)/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) 
	rm -rf $(TESTS_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/$(SRC_DIR)

$(TESTS_DIR):
	mkdir -p $(TESTS_DIR)
