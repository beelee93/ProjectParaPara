//////////////////////////////////////////
// GOShockwave.cpp
// Implements the shockwave effect
//////////////////////////////////////////

#include "Globals.h"

GOShockwave::GOShockwave() : BL_GameObject(OBJ_SHOCKWAVE) { }

void GOShockwave::OnInit(int id, int type, void* data)
{
    this->sprite = GetSpriteList()->GetSprite(OBJ_SHOCKWAVE);
    firstUpdate = 1;
    alpha = 0;
}

void GOShockwave::OnUpdate(double secs)
{
    if(firstUpdate)
    {
        xstart = x;
        ystart = y;
        firstUpdate = 0;
        alpha = 1;
    }

    imageScaleX += 3*secs;
    imageScaleY += 3*secs;

    x=xstart + 32 - (imageScaleX*32);
    y=ystart + 32 - (imageScaleX*32);

    alpha-=2*secs;
    if(alpha<=0)
        SignalDestroy();
}
