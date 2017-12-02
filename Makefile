CC = gcc
CFLAGS = -W -Wall -pedantic -g
LDFLAGS = -lusb-1.0
EXEC = USB
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: $(EXEC)

main: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o
cleanall: clean
	rm -rf $(EXEC)
