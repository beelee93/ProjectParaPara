//////////////////////////////////////////
// GOShockwave.cpp
// Implements the shockwave effect
//////////////////////////////////////////

#include "Globals.h"

GOShockwave::GOShockwave() : BL_GameObject(OBJ_SHOCKWAVE) { }

// create this object at a position which will be the centre of the shockwave
void GOShockwave::OnInit(int id, int type, void* data)
{
	depth = -5;
    this->sprite = GetSpriteList()->GetSprite(OBJ_SHOCKWAVE);
    firstUpdate = 1;
	imageScaleX = imageScaleY = 0.1;
	scaler = 5;
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

    imageScaleX += scaler*secs;
    imageScaleY += scaler*secs;
	scaler *= SDL_pow(2.0, -secs);

    x=xstart - (imageScaleX*128);
    y=ystart - (imageScaleX*128);

    alpha-=2*secs;
    if(alpha<=0)
        SignalDestroy();
}
