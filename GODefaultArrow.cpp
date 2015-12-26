//////////////////////////////////////////
// GODefaultArrow.cpp
// Inherits the basic GameObject for
// Default Arrows
//////////////////////////////////////////

#include "Globals.h"

GODefaultArrow::GODefaultArrow() : BL_GameObject(OBJ_DEFAULT_ARROWS) { }

void GODefaultArrow::OnInit(int id, int type)
{
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

    if(y<-64)
       SignalDestroy();
}
