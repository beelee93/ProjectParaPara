//////////////////////////////////////////
// GameObjectManager.c
// Implements the game object manager
//////////////////////////////////////////
#include "GameObjectManager.h"

static SDL_Rect tempRect[2];

// Initiliases the GOM
GOM::GOM()
{
    capacity = 20;
    objCount = 0;
    objects = (GameObject**)malloc(sizeof(GameObject*)*20);

    mainRenderer = NULL;

    // zero out everything (set all to null pointers)
    memset(objects, 0, sizeof(GameObject*)*20);

    // create render queue
    renderQueue = new RenderQueue();
}

// Free the GOM
GOM::~GOM()
{
	// destroy and free all objects
    DestroyAllObjects();

	delete renderQueue;

    // now free the list
    free( objects );
}

// Creates an object of specified type with its default
// settings, and returns a pointer to it
GameObject* GOM::CreateObject(int type, void* data)
{
    GameObject *obj = NULL;
    int i=0;

    // we check capacity
    if(objCount >= capacity)
    {
        // start searching for empty slots
        // at appropriate position
        i=capacity;

        // we have to expand capacity
        size_t sz = sizeof(GameObject*);
        int newCap = capacity + CAPACITY_INC;
        GameObject** tempObjs = (GameObject**)malloc(
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
                obj = new GameObject(OBJ_NULL);
            else
                obj = OnCreateObject(type);

            obj->OnInit(i, type, data);
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
void GOM::DestroyObjectById(int id)
{
    GameObject* obj;

    if(id<0 || id>=capacity) return;

    obj=objects[id];
    if(!obj) return;    // nothing to destroy

    delete obj;
    objects[id] = NULL;

    objCount--;
}

// Destroys  and frees all objects in the list
void GOM::DestroyAllObjects()
{
    int i;
    GameObject* obj;

    for(i=0;i<capacity;i++)
    {
        if((obj=objects[i]))
        {
            delete obj;
            objects[i] = NULL;
        }
    }

    objCount=0;

    // purge render queue to prevent rendering
    renderQueue->Purge();
}

// Updates all active objects in the list
void GOM::Update(double secs)
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
        // queue object for rendering
        renderQueue->QueueObject(objects[i]);
        objects[i]->OnUpdate(secs);
    }
}

// Renders all active objects in the list
void GOM::Render(double secs)
{
    renderQueue->Render(secs, mainRenderer);
}

GameObject** GOM::FindObjectsOfType(int type, SDL_Rect* searchArea)
{
    int i=0, j=0;;
    GameObject* obj;

    // Iterate through
    for(i=0;i<capacity && j<26;i++)
    {
        obj=objects[i];
        if(obj==NULL) continue;
        if(obj->type != type) continue;
        if(searchArea)
        {
            tempRect[0] = obj->sprite->GetRect(obj->imageIndex);
            tempRect[0].x = obj->x;
            tempRect[0].y = obj->y;

            if( !SDL_IntersectRect(tempRect, searchArea, tempRect+1) )
                continue;
        }
        tempArray[j++] = obj;
    }
    tempArray[j] = NULL;
    return tempArray;
}

GameObject* GOM::OnCreateObject(int type)
{
    return new GameObject(OBJ_NULL);
}

int GOM::GetObjectCount()
{
    return objCount;
}

int GOM::GetCapacity()
{
    return capacity;
}

void GOM::SetAttachedRenderer(SDL_Renderer* renderer)
{
    mainRenderer = renderer;
}

SDL_Renderer* GOM::GetAttachedRenderer()
{
    return mainRenderer;
}
