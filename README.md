#Project Para Para

This project aims to replicate the Para Para Machine
in arcades using Raspberry Pi.

---

#Required Libraries

1. SDL2             - main rendering
2. SDL2_Image       - loading png images
3. irrklang         - for audio
4. SDL_FontCache    - For optimised text rendering

---

#Source code summary

###Main.cpp

Contains the main entry point of the application.

###Game.cpp

Implementation of a basic BL_Game class, that
will support the creation of the game window
and the main game loop.
Inherit this class to customise the update
and render cycles.

Inherited by:
* GameParaPara

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
