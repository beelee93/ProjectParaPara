//////////////////////////////////////////
// GODefaultArrow.cpp
// Inherits the basic GameObject for
// Default Arrows
//////////////////////////////////////////

#include "Globals.h"

GODefaultArrow::GODefaultArrow() : BL_GameObject(OBJ_DEFAULT_ARROWS) { }

void GODefaultArrow::OnInit(int id, int type)
{
    disappearing = 0;
    this->sprite = GetSpriteList()->GetSprite(OBJ_DEFAULT_ARROWS);
    imageSpeed=0;
    imageIndex = rand() % 5;

    x = 208 + 80*imageIndex;
    y = 600;
    vy= -200;
}

void GODefaultArrow::OnUpdate(double secs)
{
    BL_GameObject::OnUpdate(secs);

    if(y<=30 && !disappearing)
    {
        disappearing = 1 ;
        y=30;
        vy=0;
    }

    if(disappearing)
    {
        alpha -= 5.0*secs;
        imageScaleX += 2*secs;
        imageScaleY += 2*secs;
        x-=2*secs * 32;
        y-=2*secs * 32;

        if(alpha<=0)
        {
            alpha=0;
            SignalDestroy();
        }
    }
}
