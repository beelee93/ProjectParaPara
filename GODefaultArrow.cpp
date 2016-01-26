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

	// Zero out the data
	SDL_memset(&this->attachedData, 0, sizeof(GODefaultArrowData));

	if (data)
		this->attachedData = *((GODefaultArrowData*)(data));

	imageIndex = HIBITS(attachedData.flags);

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

void GODefaultArrow::OnRender(double secs, SDL_Renderer* renderer)
{
    if(!visible || destroyFlag) return;

    float dy=0;
    SDL_Texture* tex;

    // draw the main sprite
    int tempImgIndex = (int)imageIndex;
    SDL_Point tempPoint;
    tempPoint.x = originX;
    tempPoint.y = originY;

    if(sprite)
    {
            tex=sprite->GetTexture();
            SDL_SetTextureAlphaMod(tex, (int)(alpha*255.0));
            SDL_SetTextureColorMod(tex, tintR, tintG, tintB);

            sprite->Render(tempImgIndex,
                (int)(x - originX),
                (int)(y - originY),
                (int)((sprite->GetRect(tempImgIndex).w) * imageScaleX),
                (int)((sprite->GetRect(tempImgIndex).h) * imageScaleY),
                imageAngle, &tempPoint, SDL_FLIP_NONE);
    }

    if(LOBITS(this->attachedData.flags))
    {
        // calculate end of chain y-ordinate
        dy = y + this->attachedData.chainDelay * ARROW_SPEED;

        // render end of chain arrow
        sprite->Render(tempImgIndex,
                (int)(x - originX),
                (int)(dy - originY),
                (int)((sprite->GetRect(tempImgIndex).w) * imageScaleX),
                (int)((sprite->GetRect(tempImgIndex).h) * imageScaleY),
                imageAngle, &tempPoint, SDL_FLIP_NONE);

        // render chain
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, (int)(alpha*255.0));

        // normal disappearing sequence
        if(disappearing)
            SDL_RenderDrawLine(renderer, xs+32, ys+32, xs+32, (int)(dy + 32));
        else
            SDL_RenderDrawLine(renderer, x+32, y+32, x+32, (int)(dy + 32));
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
