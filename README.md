#Project Para Para

This project aims to replicate the Para Para Machine
in arcades using Raspberry Pi.

---

#Required Libraries

|   |Library            |Description                     |
|---|-------------------|--------------------------------|
|1  |SDL2               |Main rendering                  |
|2  |SDL2_Image         |Loading png images              |
|3  |SDL2_TTF           |Font rendering support          |
|4  |SDL2_Mixer         |Audio                           |
|5  |SDL_FontCache      |Optimised text rendering        |
|6  |WiringPi		    |To interface GPIO pins          |

---

#Compilation

First, ensure that the required libraries are installed.

To compile and run the game without interfacing with the 
RPi GPIO pins:
```
make
./para
```

To compile and run the game with the RPi GPIO pins
```
make rpi=1
sudo ./para
```

---

#Source code summary

###Main.cpp

Contains the main entry point of the application.

###Game.cpp

Implementation of a basic Game class, that
will support the creation of the game window
and the main game loop.
Inherit this class to customise the update
and render cycles.

Inherited by:
* GameParaPara

###Audio.cpp

Implements a wrapper around SDL_mixer to simplify 
its usage within the game

###Sprite.cpp

Implements the class that will store information
about a sprite sheet.

###GameObject.cpp

Base object from which all in-game objects will
inherit.

Inherited by:
* GODefaultArrow
* GOStationaryArrow
* GOShockwave
* GOPinkFlash

###GameObjectManager.cpp

Manages the creation and destruction of objects
during the course of the game.
This should be inherited to, most importantly,
customise the OnCreateObject function.

Inherited by:
* GOMParaPara

###SpriteLoader.cpp

Contains a class that will store all loaded
sprites. It can load information from a special
input file (sprite definitions file).

###RenderQueue.cpp

Implements a sorted linked list to be used by the
game object manager during rendering cycle.

###InputParaPara.cpp

Contains implementations to interface keyboard (and GPIO)