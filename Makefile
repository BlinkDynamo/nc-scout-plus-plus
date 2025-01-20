CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
OBJ_DIR = build
SRC_DIR = src
SRCS = src/nc-scout.c src/validate.c src/naming.c
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
EXEC = build/nc-scout

all: $(OBJ_DIR) $(EXEC) 

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

$(OBJ_DIR)/$(SRC_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) 

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/$(SRC_DIR)
