//////////////////////////////////////////
// GameObjectManager.c
// Implements the game object manager
//////////////////////////////////////////
#include "GameObjectManager.h"

// Initiliases the GOM
BL_GOM::BL_GOM()
{
    capacity = 20;
    objCount = 0;
    objects = (BL_GameObject**)malloc(sizeof(BL_GameObject*)*20);

    // zero out everything (set all to null pointers)
    memset(objects, 0, sizeof(BL_GameObject*)*20);
}

// Free the GOM
BL_GOM::~BL_GOM()
{
    // destroy and free all objects
    DestroyAllObjects();

    // now free the list
    free( objects );
}

// Creates an object of specified type with its default
// settings, and returns a pointer to it
BL_GameObject* BL_GOM::CreateObject(int type)
{
    BL_GameObject *obj = NULL;
    int i=0;

    // we check capacity
    if(objCount >= capacity)
    {
        // start searching for empty slots
        // at appropriate position
        i=capacity;

        // we have to expand capacity
        size_t sz = sizeof(BL_GameObject*);
        int newCap = capacity + CAPACITY_INC;
        BL_GameObject** tempObjs = (BL_GameObject**)malloc(
                                     sz * newCap);

        // set all to zero
        memset(tempObjs, 0, sz * newCap);

        // copy over
        memcpy(tempObjs, objects, sz * capacity);

        // free the old pointer
        free (objects);

        // set the new pointer
        objects = tempObjs;
        capacity = newCap;
    }

    // at this point, there are empty slots
    // we find the first empty slot
    for(;i<capacity;i++)
    {
        if(objects[i]==NULL)
        {
            // init object
            if(type==OBJ_NULL)
                obj = new BL_GameObject(OBJ_NULL);
            else
                obj = OnCreateObject(type);

            obj->OnInit(i, type);
            obj->SetAttachedGOM(this);

            // put this object into manager
            objects[i] = obj;

            objCount++;
            return obj;
        }
    }

    // for some weird reason that we reach here
    // return a null ptr
    return NULL;
}

// Destroys and frees an object based on its id
void BL_GOM::DestroyObjectById(int id)
{
    BL_GameObject* obj;

    if(id<0 || id>=capacity) return;

    obj=objects[id];
    if(!obj) return;    // nothing to destroy

    delete obj;
    objects[id] = NULL;

    objCount--;
}

// Destroys  and frees all objects in the list
void BL_GOM::DestroyAllObjects()
{
    int i;
    BL_GameObject* obj;

    for(i=0;i<capacity;i++)
    {
        if((obj=objects[i]))
        {
            delete obj;
            objects[i] = NULL;
        }
    }

    objCount=0;
}

// Updates all active objects in the list
void BL_GOM::Update(double secs)
{
    int i;
    for (i=0;i<capacity;i++)
    {
        if(!objects[i]) continue;
        if(objects[i]->IsBeingDestroyed())
        {
            DestroyObjectById(i);
            continue;
        }
        objects[i]->OnUpdate(secs);
    }
}

// Renders all active objects in the list
void BL_GOM::Render(double secs)
{
    int i;
    for (i=0;i<capacity;i++)
    {
        if(!objects[i]) continue;
        if(objects[i]->IsBeingDestroyed()) continue;
        objects[i]->OnRender(secs);
    }
}

BL_GameObject* BL_GOM::OnCreateObject(int type)
{
    return new BL_GameObject(OBJ_NULL);
}

int BL_GOM::GetObjectCount()
{
    return objCount;
}

int BL_GOM::GetCapacity()
{
    return capacity;
}
