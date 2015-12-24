//////////////////////////////////////////
// GameObject.c
// Implements the basic in-game object
//////////////////////////////////////////
#include "GameObject.h"

static int tempImgIndex;
static BL_Sprite* tempSpr;
static SDL_Point tempPoint;

static double WrapDouble(double v, double max)
{
    if(v<0)
    {
        while(v<0)
            v+=max;
        return v;
    }
    else
    {
        while(v>=max)
            v-=max;
        return v;
    }
}

// Sets specified object to default settings (null object)
void BL_ObjectDefault(BL_GameObject* obj)
{
    if(!obj)
        return;

    obj->type = OBJ_NULL;
    obj->id = 0;
    obj->enabled = 1;
    obj->visible = 1;
    obj->alpha = 255;
    obj->tintB = obj->tintG = obj->tintR = 255;
    obj->imageIndex = 0;
    obj->imageSpeed = 1;
    obj->sprite = NULL;
    obj->x = obj->y = 0;
    obj->vx = obj->vy = 0;
    obj->useStandardUpdate = 1;
    obj->imageScaleX = obj->imageScaleY = 1.0;
    obj->originX = obj->originY = 0;
    obj->imageAngle = 0;
}

////////////////////////////////////////////////////////////////
// Sets obj to have the settings of
// specified object type
// Sprite is not set!
void BL_ObjectInit(BL_GameObject* obj, int type)
{
    // Set default settings first
    BL_ObjectDefault(obj);
    obj->type = type;

    // Now populate with the correct settings
    switch(type)
    {
        case OBJ_NULL:      // Null object. Nothing special.
            break;
    }
}

////////////////////////////////////////////////////////////////
// Performs an update on specified object
void BL_ObjectUpdate(BL_GameObject* obj, double secs)
{
    if(!obj) return;
    if(!(obj->enabled)) return;

    if(obj->useStandardUpdate)
    {
        // Standard updates
        obj->x += obj->vx * secs;
        obj->y += obj->vy * secs;

        // animate sprite only when there is one and
        // there are more than one sprite element
        if(obj->sprite && obj->sprite->elemCount>1)
        {
            obj->imageIndex += obj->imageSpeed * secs;
            obj->imageIndex = WrapDouble(obj->imageIndex, obj->sprite->elemCount);
        }
    }

    switch(obj->type)
    {
        default:
            break;
    }
}

////////////////////////////////////////////////////////////////
// Renders the object
void BL_ObjectRender(BL_GameObject* obj, double secs, SDL_Renderer* renderer)
{
    if(!obj) return;
    if(!(obj->visible)) return;

    tempImgIndex = (int)obj->imageIndex;
    tempSpr = obj->sprite;
    tempPoint.x = obj->originX;
    tempPoint.y = obj->originY;

    switch(obj->type)
    {
        default:
            if(tempSpr)
            {
                    BL_SpriteRender(renderer, tempSpr, tempImgIndex,
                        (int)(obj->x - obj->originX),
                        (int)(obj->y - obj->originY),
                        (int)((tempSpr->srcRects[tempImgIndex].w) * obj->imageScaleX),
                        (int)((tempSpr->srcRects[tempImgIndex].h) * obj->imageScaleY),
                        obj->imageAngle, &tempPoint, SDL_FLIP_NONE);
            }
            break;
    }
}
