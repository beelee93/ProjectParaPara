//////////////////////////////////////////
// GameObject.h
// Defines functions to that encapsulate
// an object/entity in game
//////////////////////////////////////////
#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED

#include "Sprite.h"
#include "Resource.h"

typedef struct __object {
    int type;               // a unique type id
    int id;                 // a unique id assigned by object manager

    int originX;           // origin of object (relative to its sprite)
    int originY;           // matters to rotation and scaling

    double x;               // position in space
    double y;

    double vx;              // velocities
    double vy;

    uint8_t alpha;           // image alpha

    uint8_t tintR;         // tint colors
    uint8_t tintG;
    uint8_t tintB;

    BL_Sprite* sprite;      // pointer to sprite associated with this object
                            // can be NULL for no sprite
    double imageIndex;      // elemIndex to be drawn. Auto-updated.
    double imageSpeed;      // speed at which image index increments

    double imageScaleX;     // image scaling
    double imageScaleY;

    double imageAngle;      // image rotation

    int enabled;            // can this object receive updates?
    int visible;            // will this object be drawn?
    int destroyed;          // has this object been destroyed?

    int useStandardUpdate;  // apply the standard updates to it?
                            // (auto update of position and imageIndex)

    void* data;             // any extra data to be stored?
} BL_GameObject;

#define OBJ_NULL -1

//////// Functions ////////
// Sets specified object to default settings (null object)
void BL_ObjectDefault(BL_GameObject* obj);

// Sets obj to have the settings of
// specified object type
void BL_ObjectInit(BL_GameObject* obj, int type);

// Performs an update on specified object
void BL_ObjectUpdate(BL_GameObject* obj, double secs);

// Renders the object
void BL_ObjectRender(BL_GameObject* obj, double secs, SDL_Renderer* renderer);

// Sets the global sprite loader. If this is not null
// and object type is not OBJ_NULL, then
// whenever an object is initialised, its sprite is set
// according to its object type
// -Create a function BL_Sprite* <funcname>(int objectType)
void BL_SetObjectSpriteLoader( BL_Sprite* (*pfnLoader)(int) );

// Sets the global object initialiser. If this is not null,
// and object type is not OBJ_NULL, then the pointer to the
// object is passed to this function
// -Create a function void <funcname>(BL_GameObject* obj, int objectType)
void BL_SetObjectInitialiser( void (*pfnInitter)(BL_GameObject*, int));

// Sets the global object updater. Non-null objects are
// passed to this
// -Create void <funcname>(BL_GameObject *obj, double seconds)
void BL_SetObjectUpdater( void (*pfnUpdater)(BL_GameObject*,double) );


#endif // GAMEOBJECT_H_INCLUDED
