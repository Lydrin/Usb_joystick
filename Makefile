CC = gcc
CFLAGS = -W -Wall -pedantic -g -I ./libusb
LDFLAGS = -lusb-1.0 -lncurses
EXEC = snake
SRC = $(wildcard libusb/*.c) $(wildcard SimpleSnake2/*.c)
OBJ = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(OBJ)
cleanall: clean
	rm -rf $(EXEC)
