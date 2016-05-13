//////////////////////////////////////////
// Sprite.h
// Implements functions to handle
// spritesheets
//////////////////////////////////////////

#include "Sprite.h"

static SDL_Rect destRect;
static SDL_Rect srcRectTemp;

static char msg[512] = "";

Sprite::Sprite(SDL_Renderer* renderer, const char* filename, int elementCount)
{
    this->renderer = renderer;
    this->elemCount = elementCount;

    SDL_Surface *surface = IMG_Load(filename);

    if(!surface)
    {
        sprintf(msg, "Sprite(): Cannot load '%s'\n", filename);
        EHLog(msg);
        goto err;
    }

    this->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if(!(this->texture))
    {
        sprintf(msg, "Sprite(): Cannot create texture '%s'\n", filename);
        EHLog(msg);
        goto err;
    }

    // allocate rects
    if(elemCount<1) elemCount = 1;
    srcRects = (SDL_Rect*)malloc(sizeof(SDL_Rect) * elemCount);

    if(srcRects == NULL)
    {
        sprintf(msg, "Sprite(): Cannot alloc srcRects '%s'\n", filename);
        EHLog(msg);

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
Sprite::~Sprite()
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
void Sprite::Render(int elemIndex, int dx, int dy, int dw, int dh,
                       double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    destRect.x=dx;
    destRect.y=dy;
    destRect.w=dw;
    destRect.h=dh;
    SDL_RenderCopyEx(renderer, texture, &(srcRects[elemIndex]),
        &destRect, angle, center, flip);
}

// Renders the clipped part of the specified element. Source dimensions are relative to the element.
void Sprite::RenderClipped(int elemIndex, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh,
	double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	srcRectTemp = srcRects[elemIndex];
	srcRectTemp.x += sx;
	srcRectTemp.y += sy;
	srcRectTemp.w = sw;
	srcRectTemp.h = sh;
	destRect.x = dx;
	destRect.y = dy;
	destRect.w = dw;
	destRect.h = dh;
	SDL_RenderCopyEx(renderer, texture, &srcRectTemp,
		&destRect, angle, center, flip);
}

void Sprite::SetRect(int elemIndex, int x, int y, int w, int h)
{
    if(elemIndex < elemCount && elemIndex>=0)
    {
        srcRects[elemIndex].x=x;
        srcRects[elemIndex].y=y;
        srcRects[elemIndex].w=w;
        srcRects[elemIndex].h=h;
    }
    else
        EHLog("Sprite::SetRect(): Index out of bounds.\n");
}

int Sprite::GetCount()
{
    return elemCount;
}
SDL_Texture* Sprite::GetTexture()
{
    return texture;
}
SDL_Rect Sprite::GetRect(int elem)
{
    return srcRects[elem];
}
int Sprite::GetInitialised()
{
    return initialised;
}
