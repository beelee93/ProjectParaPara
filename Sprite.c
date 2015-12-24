//////////////////////////////////////////
// Sprite.h
// Implements functions to handle
// spritesheets
//////////////////////////////////////////

#include "Sprite.h"
static SDL_Rect destRect;
static char msg[512] = "";

// Creates and sets the sprite
int BL_CreateSprite(SDL_Renderer* renderer, const char* filename, BL_Sprite* sprite, int elemCount)
{

    SDL_Surface *surface = IMG_Load(filename);

    if(!surface)
    {
        sprintf(msg, "CreateSprite(): Cannot load '%s'\n", filename);
        BL_EHLog(msg);
        return 0;
    }

    sprite->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if(!(sprite->texture))
    {
        sprintf(msg, "CreateSprite(): Cannot create texture '%s'\n", filename);
        BL_EHLog(msg);
        return 0;
    }

    // allocate rects
    if(elemCount<1) elemCount = 1;
    sprite->srcRects = (SDL_Rect*)malloc(sizeof(SDL_Rect) * elemCount);

    sprite->elemCount = elemCount;

    return 1;
}

// Removes sprite from memory
void BL_DestroySprite(BL_Sprite* sprite)
{

}

// Renders the sprite using the specified renderer and properties
void BL_RenderSprite(SDL_Renderer *renderer, BL_Sprite* sprite, int elemIndex,
    int dx, int dy, int dw, int dh, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    destRect.x=dx;
    destRect.y=dy;
    destRect.w=dw;
    destRect.h=dh;
    SDL_RenderCopyEx(renderer, sprite->texture, &(sprite->srcRects[elemIndex]),
        &destRect, angle, center, flip);
}
