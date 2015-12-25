//////////////////////////////////////////
// SpriteLoader.h
// Contains the definition for a sprite
// definition file loader
//////////////////////////////////////////
#ifndef SPRITELOADER_H_INCLUDED
#define SPRITELOADER_H_INCLUDED

#include "Sprite.h"
#include "ErrorHandler.h"
#include <stdio.h>

#define CMD_SET_COUNT   0
#define CMD_SET_SPRITE  1
#define CMD_SET_RECT    2
#define CMD_NULL        -1

typedef struct  {
    BL_Sprite* list;
    int count;
} BL_SpriteList;

int BL_SpriteLoadDefinitions(SDL_Renderer* renderer, const char* filename, BL_SpriteList* sprList);
void BL_SpriteFreeDefinitions(BL_SpriteList* sprList);
#endif // SPRITELOADER_H_INCLUDED
