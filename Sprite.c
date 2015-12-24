//////////////////////////////////////////
// Sprite.h
// Implements functions to handle
// spritesheets
//////////////////////////////////////////

#include "Sprite.h"
static SDL_Rect destRect;
static char msg[512] = "";

// Creates and sets the sprite
int BL_SpriteCreate(SDL_Renderer* renderer, const char* filename, BL_Sprite* sprite, int elemCount)
{
    SDL_Surface *surface = IMG_Load(filename);

    if(!surface)
    {
        sprintf(msg, "SpriteCreate(): Cannot load '%s'\n", filename);
        BL_EHLog(msg);
        return 0;
    }

    sprite->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if(!(sprite->texture))
    {
        sprintf(msg, "SpriteCreate(): Cannot create texture '%s'\n", filename);
        BL_EHLog(msg);
        return 0;
    }

    // allocate rects
    if(elemCount<1) elemCount = 1;
    sprite->srcRects = (SDL_Rect*)malloc(sizeof(SDL_Rect) * elemCount);

    if(sprite->srcRects == NULL)
    {
        sprintf(msg, "SpriteCreate(): Cannot alloc srcRects '%s'\n", filename);
        BL_EHLog(msg);

        SDL_DestroyTexture(sprite->texture);
        sprite->texture = NULL;

        return 0;
    }
    sprite->elemCount = elemCount;

    return 1;
}

// Removes sprite from memory
void BL_SpriteDestroy(BL_Sprite* sprite)
{
    if(sprite->texture)
    {
        SDL_DestroyTexture(sprite->texture);
        sprite->texture = NULL;
    }

    if(sprite->srcRects)
    {
        free(sprite->srcRects);
        sprite->srcRects = NULL;
    }
}

// Renders the sprite using the specified renderer and properties
void BL_SpriteRender(SDL_Renderer *renderer, BL_Sprite* sprite, int elemIndex,
    int dx, int dy, int dw, int dh, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    destRect.x=dx;
    destRect.y=dy;
    destRect.w=dw;
    destRect.h=dh;
    SDL_RenderCopyEx(renderer, sprite->texture, &(sprite->srcRects[elemIndex]),
        &destRect, angle, center, flip);
}

void BL_SpriteSetRect(BL_Sprite* sprite, int elemIndex, int x, int y, int w, int h)
{
    if(elemIndex < (sprite->elemCount) && elemIndex>=0)
    {
        sprite->srcRects[elemIndex].x=x;
        sprite->srcRects[elemIndex].y=y;
        sprite->srcRects[elemIndex].w=w;
        sprite->srcRects[elemIndex].h=h;
    }
    else
        BL_EHLog("SpriteSetRect(): Index out of bounds.\n");
}
