#!/bin/bash

echo ---------------------------------------------
echo \* SimpleSnake 1.0               
echo \* by Antoine Duquenoy and Anthony Durot
echo ---------------------------------------------

isInstalled=`dpkg -s libncurses5 | grep Status | cut -d' ' -f3`

if [ $isInstalled = "ok" ]
then
	echo "NCURSES 5 is already installed"

else
	echo "NCURSES 5 is not installed"
	sudo apt-get install libncurses5-dev		
fi

gcc -o game game.c -lncurses
echo Start the game [y\/n]
read yn
if [ $yn = "y" ] || [ $yn = "Y" ]
then
	./game
else
	echo To play later .\/game
fi

exit 0
