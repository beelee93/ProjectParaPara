//////////////////////////////////////////
// GameObject.c
// Implements the basic in-game object
//////////////////////////////////////////
#include "GameObject.h"


static int tempImgIndex;
static Sprite* tempSpr;
static SDL_Point tempPoint;

static double WrapDouble(double v, double max)
{
    if(v<0)
    {
        while(v<0)
            v+=max;
        return v;
    }
    else
    {
        while(v>=max)
            v-=max;
        return v;
    }
}

GameObject::GameObject(int type)
{
    OnInit(0, type);
}

GameObject::~GameObject() { }

void GameObject::OnInit(int id, int type, void* data)
{
    this->type = type;
    this->id = id;
    enabled = 1;
    visible = 1;
    alpha = 1;
    tintB = tintG = tintR = 255;
    imageIndex = 0;
    imageSpeed = 0;
    sprite = NULL;
    x = y = 0;
    vx = vy = 0;
    imageScaleX = imageScaleY = 1.0;
    originX = originY = 0;
    imageAngle = 0;
    destroyFlag = 0;
    depth = 0;
}

void GameObject::OnRender(double secs, SDL_Renderer* renderer)
{
    if(!visible || destroyFlag) return;

    SDL_Texture* tex;
    tempImgIndex = (int)imageIndex;
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
}
void GameObject::OnUpdate(double secs)
{
    if(!enabled || destroyFlag) return;

    // Standard updates
    x += vx * secs;
    y += vy * secs;

    // animate sprite only when there is one and
    // there are more than one sprite element
    if(sprite && sprite->GetCount()>1)
    {
        imageIndex += imageSpeed * secs;
        imageIndex = WrapDouble(imageIndex, sprite->GetCount());
    }
}

void GameObject::SignalDestroy()
{
    destroyFlag = 1;
}

int GameObject::IsBeingDestroyed()
{
    return destroyFlag;
}

void* GameObject::GetAttachedGOM()
{
    return attachedGOM;
}

void GameObject::SetAttachedGOM(void* gom)
{
    attachedGOM = gom;
}
