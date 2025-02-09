#----------------------------------------------------------------------------------------------#
#
# @author       J. Hayden
# @file         Makefile
# @license      GPL-3.0
#
#----------------------------------------------------------------------------------------------#

CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Iinclude 

# Installation prefix.
PREFIX ?= /usr/local

# Important directories.
OBJ_DIR = build
SRC_DIR = src
TEST_DIR = tests
BIN_DIR = $(PREFIX)/bin

# Important files.
SRCS = src/nc-scout.c src/validate.c src/naming.c src/search.c	
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
EXEC = build/nc-scout
TEST_BUILDSCRIPT = scripts/build-tests.sh

# Dependency tree.
#----------------------------------------------------------------------------------------------#
all: $(OBJ_DIR) $(EXEC)

check: $(TEST_DIR) $(OBJ_DIR) $(EXEC)
	$(TEST_BUILDSCRIPT)  

install: $(EXEC)
	install -d $(DESTDIR)$(BIN_DIR)
	install -m 755 $(EXEC) $(DESTDIR)$(BIN_DIR)

uninstall:
	rm -f $(DESTDIR)$(BIN_DIR)/$(notdir $(EXEC))

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

$(OBJ_DIR)/$(SRC_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) 
	rm -rf $(TEST_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/$(SRC_DIR)

$(TEST_DIR):
	mkdir -p $(TEST_DIR)
