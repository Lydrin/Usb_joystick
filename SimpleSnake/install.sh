#!/bin/bash

clear
echo ---------------------------------------------
echo \* SimpleSnake 1.3            
echo \* by Antoine Duquenoy and Anthony Durot
echo ---------------------------------------------

isInstalled=`dpkg -s libncurses5 | grep Status | cut -d' ' -f3`

if [ $isInstalled = "ok" ]
then
	echo -e "\n"
	echo "NCURSES 5 is already installed"
	echo -e "\n"
else
	echo -e "\n"
	echo "NCURSES 5 is not installed"
	sudo apt-get install libncurses5-dev
	echo -e "\n"
fi

gcc -o game game.c -lncurses
echo "-------- INSTRUCTIONS --------"
echo "* Use the arrow keys to move"
echo "* ESCAPE to pause"
echo "------------------------------"
echo -e "\n"
read -p "Start the game [y/n] : " yn
if [ $yn = "y" ] || [ $yn = "Y" ]
then
	./game
else
	echo To play later .\/game
fi

exit 0
