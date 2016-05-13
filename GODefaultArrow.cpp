//////////////////////////////////////////
// GODefaultArrow.cpp
// Inherits the basic GameObject for
// Default Arrows
//////////////////////////////////////////

#include "Globals.h"

GODefaultArrow::GODefaultArrow() : GameObject(OBJ_DEFAULT_ARROWS) { }

void GODefaultArrow::OnInit(int id, int type, void* data)
{
	hasInput = 0;
	this->failedCallback = NULL;
	this->chainInput = 1;
	this->chainSuccess = 0;
	this->chainBroken = 0;
	this->chainBrokenAcknowledged = 0;

    disappearing = 0;
    this->sprite = GetSpriteList()->GetSprite(OBJ_DEFAULT_ARROWS);
    imageSpeed=0;

	// Zero out the data
	SDL_memset(&this->attachedData, 0, sizeof(GODefaultArrowData));

	if (data)
		this->attachedData = *((GODefaultArrowData*)(data));

	imageIndex = HIBITS(attachedData.flags);

    x = 208 + 80*imageIndex;
	colX = x;
    y = 600;
    vy= -ARROW_SPEED;
}

void GODefaultArrow::OnUpdate(double secs)
{
    GameObject::OnUpdate(secs);

	if (IsChained())
	{
		if (HasInput() && !chainBroken)
		{
			disappearing = 0;
			alpha = 1.0;
			imageScaleX = 1.0;
			imageScaleY = 1.0;
			x = colX;
			y = ARROW_TARGET_Y;
			if (this->chainInput)
			{
				this->attachedData.chainDelay -= secs;
			}
			else // chain is broken
			{
				chainBroken = 1;
				Disappear();
			}

			if (this->attachedData.chainDelay < 0)
			{
				this->attachedData.chainDelay = 0;
				this->chainSuccess = 1;
				chainBroken = 1;
			}
		}
		else
		{
			if (!disappearing && y <= ARROW_TARGET_Y)
			{
				y = ARROW_TARGET_Y;
				Disappear();
			}
		}
	}
	else
	{
		if (!disappearing && y <= ARROW_TARGET_Y)
		{
			y = ARROW_TARGET_Y;
			Disappear();
		}
	}

    if(disappearing && alpha>0)
    {
        alpha -= 10.0*secs;
        imageScaleX += 9*secs;
        imageScaleY += 9*secs;
		x = (int)(xs + 32 - imageScaleX * 32);
		y = (int)(ys + 32 - imageScaleY * 32);

        if(alpha<0)
        {
            alpha=0;
            SignalDestroy();
			if (this->failedCallback)
			{
				if(!this->hasInput)	this->failedCallback(this);
				else if (this->IsChained() && this->attachedData.chainDelay>0) this->failedCallback(this);
			}
        }
    }
}

void GODefaultArrow::SetAccuracy(double accu)
{
	accuracy = accu;
}

double GODefaultArrow::GetAccuracy()
{
	return accuracy;
}

void GODefaultArrow::FlagForChain(int flag)
{
	chainInput = flag;
}

void GODefaultArrow::OnRender(double secs, SDL_Renderer* renderer)
{
    if(!visible || destroyFlag) return;

    float dy=0;
	int tempH;

    SDL_Texture* tex;

	// draw the main sprite
	int tempImgIndex = (int)imageIndex;

	if (IsChained())
	{
		// draw chain
		static Sprite* sprBack = GetSpriteList()->GetSprite(SPR_CHAINED_BACK);

		// calculate end of chain y-ordinate
		tex = sprBack->GetTexture();
		dy = y + this->attachedData.chainDelay * ARROW_SPEED;
		
		// render chain by tiling
		SDL_SetTextureAlphaMod(tex, (int)(alpha*255.0));
		SDL_SetTextureColorMod(tex, tintR, tintG, tintB);
		int ay;
		for (ay = y+32; ay + 32 < dy; ay += 64 )
		{
			sprBack->Render(0,colX+7, ay,50,64,0,NULL,SDL_FLIP_NONE);			
		}
		// render remainder
		if ((tempH = dy + 32 - ay) > 0)
		{
			sprBack->RenderClipped(0, 0,0, 64, tempH, 
				colX +7, ay, 50, tempH, 0, NULL, SDL_FLIP_NONE);
		}

		// render end of chain arrow
		tex = sprite->GetTexture();
		SDL_SetTextureAlphaMod(tex, (int)(alpha*255.0));
		SDL_SetTextureColorMod(tex, tintR, tintG, tintB);

		sprite->Render(tempImgIndex,
			(int)x,
			(int)dy,
			(int)((sprite->GetRect(tempImgIndex).w) * imageScaleX),
			(int)((sprite->GetRect(tempImgIndex).h) * imageScaleY),
			imageAngle, NULL, SDL_FLIP_NONE);
	}

    if(sprite)
    {
            tex=sprite->GetTexture();
            SDL_SetTextureAlphaMod(tex, (int)(alpha*255.0));
            SDL_SetTextureColorMod(tex, tintR, tintG, tintB);

            sprite->Render(tempImgIndex,
				(int)x,
                (int)y,
                (int)((sprite->GetRect(tempImgIndex).w) * imageScaleX),
                (int)((sprite->GetRect(tempImgIndex).h) * imageScaleY),
                imageAngle, NULL, SDL_FLIP_NONE);
    }

}

int GODefaultArrow::IsChained()
{
	return LOBITS(this->attachedData.flags);
}

int GODefaultArrow::IsChainBroken()
{
	int res = this->chainBroken;
	if (!res) return res;

	if (this->chainBrokenAcknowledged) return 0;
	else return 1;
}

int GODefaultArrow::GetChainSuccess()
{
	if (chainSuccess)
	{
		chainSuccess = 0;
		return 1;
	}
	else
		return 0;
}

void GODefaultArrow::SetInput()
{
	hasInput = 1;
	if(!IsChained()) 
		Disappear(); // not-chained. so just disappear
}

int GODefaultArrow::IsDisappearing()
{
	return disappearing;
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

void GODefaultArrow::SetFailedCallback(void(*callback)(GODefaultArrow*))
{
	this->failedCallback = callback;
}