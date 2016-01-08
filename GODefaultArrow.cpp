//////////////////////////////////////////
// GODefaultArrow.cpp
// Inherits the basic GameObject for
// Default Arrows
//////////////////////////////////////////

#include "Globals.h"

GODefaultArrow::GODefaultArrow() : BL_GameObject(OBJ_DEFAULT_ARROWS) { }

void GODefaultArrow::OnInit(int id, int type, void* data)
{
	hasInput = 0;
    disappearing = 0;
    this->sprite = GetSpriteList()->GetSprite(OBJ_DEFAULT_ARROWS);
    imageSpeed=0;
    if(!data)
        imageIndex = rand() % 5;
    else
        imageIndex = *((int*)data);
    x = 208 + 80*imageIndex;
    y = 600;
    vy= -ARROW_SPEED;
}

void GODefaultArrow::OnUpdate(double secs)
{
    BL_GameObject::OnUpdate(secs);

    if(y<=ARROW_TARGET_Y && !disappearing)
    {
        disappearing = 1 ;
        y=ARROW_TARGET_Y;
		xs = x;
		ys = y;
        vy=0;
    }

    if(disappearing)
    {
        alpha -= 10.0*secs;
        imageScaleX += 9*secs;
        imageScaleY += 9*secs;
		x = (int)(xs + 32 - imageScaleX * 32);
		y = (int)(ys + 32 - imageScaleY * 32);

        if(alpha<=0)
        {
            alpha=0;
            SignalDestroy();
        }
    }
}

void GODefaultArrow::SetInput()
{
	hasInput = 1;
	Disappear();
}

int GODefaultArrow::HasInput()
{
	return hasInput;
}

void GODefaultArrow::Disappear()
{
    disappearing = 1;
    vy = 0;
	xs = x;
	ys = y;
}
