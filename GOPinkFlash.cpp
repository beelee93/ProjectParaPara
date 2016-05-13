//////////////////////////////////////////
// GOPinkFlash.cpp
// Inherits the basic GameObject for
// a pink flash column
//////////////////////////////////////////

#include "Globals.h"

GOPinkFlash::GOPinkFlash() : GameObject(OBJ_PINK_FLASH) {}

void GOPinkFlash::OnInit(int id, int type, void* data)
{
   if(data)
   {
       x = 208 + 80* (*(int*)data);
   }
   alpha = 0;
   drawRect.x = x;
   drawRect.y = 0;
   drawRect.w = 64;
   drawRect.h = 600;

   depth = 10;
}

void GOPinkFlash::OnUpdate(double secs)
{
    if(alpha>0)
    {
        alpha-=secs;
        if(alpha<0) alpha=0;
    }
}

void GOPinkFlash::Flash()
{
    alpha = 0.5;
}

void GOPinkFlash::OnRender(double secs, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255,0,255,(int)(alpha*255.0));
    SDL_RenderFillRect(renderer, &drawRect);
}
