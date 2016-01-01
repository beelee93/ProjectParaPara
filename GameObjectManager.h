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
#include "RenderQueue.h"
#include <stdlib.h>

#define CAPACITY_INC 20         /* rate of expansion of GOM capacity */

class BL_GOM
{
public:
    BL_GOM();
    ~BL_GOM();

    // Creates an object of specified type with its default
    // settings, and returns a pointer to it
    // can pass a data to it
    BL_GameObject* CreateObject(int type, void* data = NULL);

    // Finds and returns an array of object references of the said type
    // Up to 25 max for the implementation
    BL_GameObject** FindObjectsOfType(int type, SDL_Rect* searchArea = NULL);

    // Destroys and frees an object based on its id
    void DestroyObjectById(int id);

    // Destroys and frees all objects in the list
    void DestroyAllObjects();

    // Updates all active objects in the list
    void Update(double secs);

    // Renders all active objects in the list
    void Render(double secs);

    int GetObjectCount();
    int GetCapacity();

    SDL_Renderer* GetAttachedRenderer();
    void SetAttachedRenderer(SDL_Renderer* renderer);

    // Override this to provide conversion between
    // integer type constant to the appropriate
    // BL_GameObject descendents
    virtual BL_GameObject* OnCreateObject(int type);

protected:
    int capacity;               // capacity of the internal array
    int objCount;               // a count of number of instantiated objects
    BL_GameObject** objects;     // list of pointers to objects
    BL_GameObject* tempArray[26];  // for use by FindObjects
    BL_RenderQueue* renderQueue;
    SDL_Renderer* mainRenderer;
    void ExpandCapacity();

};

#endif
