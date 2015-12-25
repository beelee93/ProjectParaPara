//////////////////////////////////////////
// GameObjectManager.c
// Implements the game object manager
//////////////////////////////////////////
#include "GameObjectManager.h"

static void ExpandCapacity(BL_GOM* mgr);

// Initialises a game object manager
void BL_GOMInit(BL_GOM* mgr)
{
    if(!mgr) return;
    mgr->capacity = 20;
    mgr->objCount = 0;
    BL_QueueCreate(&(mgr->reusables));
    mgr->objects = (BL_GameObject**)malloc(sizeof(BL_GameObject*)*20);
    mgr->freed=0;

    // zero out everything (set all to null pointers)
    memset(mgr->objects, 0, sizeof(BL_GameObject*)*20);
}

////////////////////////////////////////////////////////////////
// Creates an object of specified type with its default
// settings, and returns a pointer to it
BL_GameObject* BL_GOMCreateObject(BL_GOM* mgr, int type)
{
    BL_GameObject *obj = NULL;
    int i=0;

    if(!mgr) return NULL;
    if(mgr->freed) return NULL;

    // check for reusables
    if(!BL_QueueIsEmpty(&mgr->reusables))
    {
        // let's reuse one
        obj = BL_QueueDequeue(&(mgr->reusables));
        i = obj->id;

        // reinitialise
        BL_ObjectInit(obj, type);
        obj->id = i;

        mgr->objCount++;

        // done!
        return obj;
    }

    // no reusables. we check capacity
    if(mgr->objCount >= mgr->capacity)
    {
        // start searching for empty slots
        // at appropriate position
        i=mgr->capacity;

        // we have to expand capacity
        ExpandCapacity(mgr);
    }

    // at this point, there are empty slots
    // we find the first empty slot
    for(;i<mgr->capacity;i++)
    {
        if(mgr->objects[i]==NULL)
        {
            obj = (BL_GameObject*)malloc(sizeof(BL_GameObject));

            // init the object
            BL_ObjectInit(obj, type);
            obj->id = i;

            // put this object into manager
            mgr->objects[i] = obj;

            mgr->objCount++;
            return obj;
        }
    }

    // for some weird reason that we reach here
    // return a null ptr
    return NULL;
}

////////////////////////////////////////////////////////////////
// Destroys an object based on its id
void BL_GOMDestroyObjectById(BL_GOM* mgr, int id)
{
    BL_GameObject* obj;

    if(!mgr) return;
    if(mgr->freed) return;
    if(id<0 || id>= mgr->capacity) return;

    obj=mgr->objects[id];
    if(!obj) return;    // nothing to destroy
    if(obj->destroyed) return; // already destroyed

    obj->destroyed = 1;

    // send this object over to reusables
    BL_QueueEnqueue(&(mgr->reusables), obj);

    mgr->objCount--;
}

////////////////////////////////////////////////////////////////
// Destroys all objects in the list
void BL_GOMDestroyAllObjects(BL_GOM* mgr)
{
    int i;
    BL_GameObject* obj;

    if(!mgr) return;
    if(mgr->freed) return;

    for(i=0;i<mgr->capacity;i++)
    {
        if((obj=mgr->objects[i]))
        {
            if(obj->destroyed) continue; // already destroyed

            obj->destroyed = 1;

            // send this object over to reusables
            BL_QueueEnqueue(&(mgr->reusables), obj);
        }
    }

    mgr->objCount=0;
}

////////////////////////////////////////////////////////////////
// Frees the memory taken up by all objects
// Including those in reusables
void BL_GOMFreeAllObjects(BL_GOM* mgr)
{
    if(!mgr) return;
    if(mgr->freed) return;

    // send everything to reusables
    BL_GOMDestroyAllObjects(mgr);

    // free everything in reusables
    while(!BL_QueueIsEmpty(&(mgr->reusables)))
        free(BL_QueueDequeue(&(mgr->reusables)));

    // set the list to contain null pointers
    memset(mgr->objects, 0, sizeof(BL_GameObject*)*mgr->capacity);
}

////////////////////////////////////////////////////////////////
// Releases the GOM from memory
void BL_GOMFree(BL_GOM* mgr)
{
    if(!mgr) return;
    if(mgr->freed) return;

    // free all objects first (frees the queue during so)
    BL_GOMFreeAllObjects(mgr);

    // now free the list
    free( mgr->objects );

    mgr->freed = 1;
}

////////////////////////////////////////////////////////////////
// Expands the capacity of the game object manager
static void ExpandCapacity(BL_GOM* mgr)
{
    BL_GameObject** list;
    int newCap = mgr->capacity + CAPACITY_INC;
    size_t sz = sizeof(BL_GameObject*);

    // 1. create a new array with bigger capacity
    list = (BL_GameObject**)malloc(sz*newCap);

    // 2. zero out everything
    memset(list, 0, sz*newCap);

    // 3. copy over the pointers from the old list to this list
    memcpy(list, mgr->objects, sz*mgr->capacity);

    // 4. free the old list
    free(mgr->objects);

    // 5. set the new list in
    mgr->objects = list;

    // 6. set the new capacity
    mgr->capacity = newCap;
}
