//////////////////////////////////////////
// GameObject.h
// Defines functions to that encapsulate
// an object/entity in game
//////////////////////////////////////////
#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED

#include "Sprite.h"

// the null object, that has no sprite and does nothing
#define OBJ_NULL        -1

class BL_GameObject
{
public:
    int type;               // a unique type id
    int id;                 // a unique id assigned by object manager

    int originX;           // origin of object (relative to its sprite)
    int originY;           // matters to rotation and scaling

    double x;               // position in space
    double y;

    double vx;              // velocities
    double vy;

    int depth;              // draw order

    double alpha;           // image alpha

    uint8_t tintR;         // tint colors
    uint8_t tintG;
    uint8_t tintB;

    double imageIndex;      // elemIndex to be drawn. Auto-updated.
    double imageSpeed;      // speed at which image index increments

    double imageScaleX;     // image scaling
    double imageScaleY;

    double imageAngle;      // image rotation

    int enabled;            // can this object receive updates?
    int visible;            // will this object be drawn?


    BL_Sprite* sprite;      // pointer to sprite associated with this object
                            // can be NULL for no sprite

    BL_GameObject(int type);
    ~BL_GameObject();

    // Be sure to call parent's functions
    virtual void OnInit(int id, int type, void* data=NULL);  // you may set your sprite here
    virtual void OnRender(double secs);
    virtual void OnUpdate(double secs);

    void SignalDestroy();
    int IsBeingDestroyed();
    void* GetAttachedGOM();
    void SetAttachedGOM(void* gom);

protected:
    int destroyFlag;        // signal for GOM to destroy this object
    void* attachedGOM;
};

#endif // GAMEOBJECT_H_INCLUDED
