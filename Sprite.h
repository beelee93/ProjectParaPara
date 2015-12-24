//////////////////////////////////////////
// Sprite.h
// Defines function prototypes needed to
// manage sprite/spritesheets
//////////////////////////////////////////

#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#include "ErrorHandler.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _sprite {
    SDL_Texture*    texture;   // pointer to texture data
    SDL_Rect*       srcRects;  // array of sheet elements
    int             elemCount; // number of elements
} BL_Sprite;

// Creates and sets the sprite
int BL_SpriteCreate(SDL_Renderer *renderer, const char* filename,
    BL_Sprite* sprite, int elemCount);

// Removes sprite from memory
void BL_SpriteDestroy(BL_Sprite* sprite);

// Renders the sprite using the specified renderer and properties
void BL_SpriteRender(SDL_Renderer* renderer, BL_Sprite* sprite, int elemIndex,
    int dx, int dy, int dw, int dh, double angle, SDL_Point* center,
    SDL_RendererFlip flip);

// Sets a sprite element's rect
void BL_SpriteSetRect(BL_Sprite* sprite, int elemIndex, int x, int y, int w, int h);
#endif // SPRITE_H_INCLUDED
