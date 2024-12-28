CC = gcc
CFLAGS = -Wall -g

all: nc-scout

nc-scout: nc-scout.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f nc-scout 
