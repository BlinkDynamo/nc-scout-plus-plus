# @author	J. Hayden
# @file		Makefile
# @license	GPL-3.0

CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Iinclude 

OBJ_DIR = build
SRC_DIR = src
TEST_DIR = tests

TEST_BUILDSCRIPT = scripts/build-tests.sh
SRCS = src/nc-scout.c src/validate.c src/naming.c src/search.c	
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
EXEC = build/nc-scout

all: $(OBJ_DIR) $(EXEC) 

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

$(OBJ_DIR)/$(SRC_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) 
	rm -rf $(TEST_DIR)

check: $(TEST_DIR)
	cd $(TEST_DIR) && ../$(TEST_BUILDSCRIPT) 

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/$(SRC_DIR)

$(TEST_DIR):
	mkdir -p $(TEST_DIR)
