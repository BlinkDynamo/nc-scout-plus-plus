CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRCS = src/nc-scout.c src/validation.c src/config.c src/naming.c
OBJS = $(SRCS:.c=.o)
EXEC = build/nc-scout

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
