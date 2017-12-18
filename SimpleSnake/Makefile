all: game

game: game.o usb_host.o
	gcc -o game game.o usb_host.o -lusb-1.0 -lncurses

game.o: game.c
	gcc -o game.o -c game.c -Wall -pedantic -lusb-1.0 -lncurses

usb_host.o: usb_host.c usb_host.h
	gcc -o usb_host.o -c usb_host.c -Wall -pedantic -lusb-1.0

clean:
	rm -rf *.o
