//////////////////////////////////////////
// Sprite.h
// Implements functions to handle
// spritesheets
//////////////////////////////////////////

#include "Sprite.h"

static SDL_Rect destRect;
static char msg[512] = "";

BL_Sprite::BL_Sprite(SDL_Renderer* renderer, const char* filename, int elementCount)
{
    this->renderer = renderer;
    this->elemCount = elementCount;

    SDL_Surface *surface = IMG_Load(filename);

    if(!surface)
    {
        sprintf(msg, "BL_Sprite(): Cannot load '%s'\n", filename);
        BL_EHLog(msg);
        goto err;
    }

    this->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if(!(this->texture))
    {
        sprintf(msg, "BL_Sprite(): Cannot create texture '%s'\n", filename);
        BL_EHLog(msg);
        goto err;
    }

    // allocate rects
    if(elemCount<1) elemCount = 1;
    srcRects = (SDL_Rect*)malloc(sizeof(SDL_Rect) * elemCount);

    if(srcRects == NULL)
    {
        sprintf(msg, "BL_Sprite(): Cannot alloc srcRects '%s'\n", filename);
        BL_EHLog(msg);

        SDL_DestroyTexture(texture);
        texture = NULL;

        goto err;
    }

    this->initialised = 1;
    return;
err:
    this->initialised = 0;
}

// dtor
BL_Sprite::~BL_Sprite()
{
    if(texture)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }

    if(srcRects)
    {
        free(srcRects);
        srcRects = NULL;
    }
}

// Renders the sprite using the specified renderer and properties
void BL_Sprite::Render(int elemIndex, int dx, int dy, int dw, int dh,
                       double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    destRect.x=dx;
    destRect.y=dy;
    destRect.w=dw;
    destRect.h=dh;
    SDL_RenderCopyEx(renderer, texture, &(srcRects[elemIndex]),
        &destRect, angle, center, flip);
}

void BL_Sprite::SetRect(int elemIndex, int x, int y, int w, int h)
{
    if(elemIndex < elemCount && elemIndex>=0)
    {
        srcRects[elemIndex].x=x;
        srcRects[elemIndex].y=y;
        srcRects[elemIndex].w=w;
        srcRects[elemIndex].h=h;
    }
    else
        BL_EHLog("BL_Sprite::SetRect(): Index out of bounds.\n");
}

int BL_Sprite::GetCount()
{
    return elemCount;
}
SDL_Texture* BL_Sprite::GetTexture()
{
    return texture;
}
SDL_Rect BL_Sprite::GetRect(int elem)
{
    return srcRects[elem];
}
int BL_Sprite::GetInitialised()
{
    return initialised;
}
