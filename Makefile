usb: fonctions.o usb_host.o
	gcc -o usb fonctions.o usb_host.o -lusb-1.0
fonctions.o: fonctions.c
	gcc -o fonctions.o -c fonctions.c -lusb-1.0
usb_host.o: usb_host.c fonctions.h
	gcc -o usb_host.o -c usb_host.c -lusb-1.0
