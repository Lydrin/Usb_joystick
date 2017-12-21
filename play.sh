#!/bin/bash

clear
echo ---------------------------------------------
echo \* SimpleSnake 2.0            
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

make cleanall all
echo -e "\n"
echo "-------- INSTRUCTIONS --------"
echo -e "\n"
echo "* Player 1 :"
echo "** Use the gamepad to move"
echo "** Joystick button to pause"
echo -e "\n"
echo "* Player 2 :"
echo "** Use the arrow keys to move"
echo "** Joystick button to pause"
echo -e "\n"
echo "------------------------------"
echo -e "\n"
read -p "Start the game [y/n] : " yn
if [ $yn = "y" ] || [ $yn = "Y" ]
then
	read -p "Numbers of players [1/2] : " p
	./snake $p
else
	echo See you \!
fi

rm snake
exit 0
