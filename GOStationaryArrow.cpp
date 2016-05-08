//////////////////////////////////////////
// GODStationaryArrow.cpp
// Inherits the basic GameObject for
// Stationary Arrows
//////////////////////////////////////////

#include "Globals.h"

GOStationaryArrow::GOStationaryArrow() : BL_GameObject(OBJ_DEFAULT_ARROWS_BW) {}

void GOStationaryArrow::OnInit(int id, int type, void* data)
{
	depth = 5;
    // set the sprite
    this->sprite = GetSpriteList()->GetSprite(OBJ_DEFAULT_ARROWS_BW);
    this->imageIndex = data? *((int*)data) : rand() % 5;

    this->y = 16;
    this->alpha = 0;
    this->x = 208 + 80*imageIndex;
    flasher = NULL;
}

void GOStationaryArrow::OnUpdate(double secs)
{
    if(y<64)
    {
        y += secs * 48.0;
        if(y>64) y=64;

        alpha = 0.5-(64 - y)/96.0;
    }
    else
    {
        if(alpha>0.5)
        {
            alpha -= secs*2;
            if(alpha<0.5) alpha = 0.5;
        }
    }
}

void GOStationaryArrow::Flash()
{
    alpha=0.8;
    if(flasher)flasher->Flash();
}

void GOStationaryArrow::SetAttachedFlash(GOPinkFlash* flash)
{
    flasher = flash;
}
