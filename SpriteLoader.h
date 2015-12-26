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
#include <string.h>

#define CMD_SET_COUNT   0
#define CMD_SET_SPRITE  1
#define CMD_SET_RECT    2
#define CMD_NULL        -1

/*
typedef struct  {
    BL_Sprite* list;
    int count;
} BL_SpriteList;

int BL_SpriteLoadDefinitions(SDL_Renderer* renderer, const char* filename, BL_SpriteList* sprList);
void BL_SpriteFreeDefinitions(BL_SpriteList* sprList);
*/

class BL_SpriteList
{
public:
    BL_SpriteList(SDL_Renderer* renderer, const char* filename);
    ~BL_SpriteList();
    int GetCount();
    int GetInitialised();
    BL_Sprite* GetSprite(int index);

private:
    BL_Sprite** list;
    int count;
    int initialised;
};
#endif // SPRITELOADER_H_INCLUDED
