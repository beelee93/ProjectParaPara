//////////////////////////////////////////
// GameObjectManager.h
// Defines function prototypes to handle
// dynamic addition/removal of game objects
//
// Destroying objects will put them into the
// reusable list to recycle them
//
// Make sure to free all objects at end of
// application
//////////////////////////////////////////
#ifndef GAMEOBJECTMANAGER_H_INCLUDED
#define GAMEOBJECTMANAGER_H_INCLUDED

#include "GameObject.h"
#include "Queue.h"
#include <stdlib.h>

#define CAPACITY_INC 20         /* rate of expansion of GOM capacity */

typedef struct __gameObjectManager {
    int capacity;               // capacity of the internal array
    int objCount;               // a count of number of instantiated objects
    BL_GameObject** objects;     // list of pointers to objects
    PtrQueue reusables;         // a queue of recently destroyed objects
    int freed;                  // has this GOM been freed?
} BL_GOM;

// Initialises a game object manager
void BL_GOMInit(BL_GOM* mgr);

// Creates an object of specified type with its default
// settings, and returns a pointer to it
BL_GameObject* BL_GOMCreateObject(BL_GOM* mgr, int type);

// Destroys an object based on its id
void BL_GOMDestroyObjectById(BL_GOM* mgr, int id);

// Destroys all objects in the list
void BL_GOMDestroyAllObjects(BL_GOM* mgr);

// Updates all active objects in the list
void BL_GOMUpdate(BL_GOM* mgr, double secs);

// Renders all active objects in the list
void BL_GOMRender(BL_GOM* mgr, SDL_Renderer* renderer, double secs);

// Frees the memory taken up by all objects
// Including those in reusables
void BL_GOMFreeAllObjects(BL_GOM* mgr);

// Releases the GOM from memory
void BL_GOMFree(BL_GOM* mgr);

#endif
