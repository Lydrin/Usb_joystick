# Tutorat Système (IMA4 - 2017/2018)
---
</br>

#### **SimpleSnake** | Contrôle d'un jeu à l'aide d'un périphérique USB 

> Projet réalisé par Anthony DUROT & Antoine DUQUENOY (Arduino 3)
>
> Utilisation de libusb, LUFA et Ncurses



## Procédure de lancement

- Assurez-vous de disposer des droits utilisateurs (requis par libusb)
- Lancez le script `play.sh` qui se charge d'installer Ncurses et de compiler les sources
- Le premier menu est utilisable avec la manette (bouton du joystick pour confirmer) et le second requiert l'utilisation du calvier (`entrer` pour valider)
- Jouez !



## Périphérique non reconnu ?

* Se rendre dans le dossier`Usb_joystick/AT16u2/PolytechLille/PAD`
* Effectuer un `make clean all`
* Flasher l'atmega16u2 avec le fichier `pad.hex`



