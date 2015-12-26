//////////////////////////////////////////
// GODownArrow.cpp
// Inherits the basic GameObject for
// Large Down Arrows
//////////////////////////////////////////

#include "Globals.h"

GODownArrow::GODownArrow() : BL_GameObject(OBJ_DOWN_ARROW)
{
    OnInit(0, OBJ_DOWN_ARROW);
}

void GODownArrow::OnInit(int id, int type)
{
    BL_GameObject::OnInit(id, type);
    this->sprite = GetSpriteList()->GetSprite(OBJ_DEFAULT_ARROWS);
}
