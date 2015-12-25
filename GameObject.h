//////////////////////////////////////////
// GameObject.h
// Defines functions to that encapsulate
// an object/entity in game
//////////////////////////////////////////
#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED

#include "Sprite.h"

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

} BL_GameObject;

//////// Object Types /////////
// An object that does nothing
#define OBJ_NULL 0

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

#endif // GAMEOBJECT_H_INCLUDED
