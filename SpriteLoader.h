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
    Sprite* list;
    int count;
} SpriteList;

int SpriteLoadDefinitions(SDL_Renderer* renderer, const char* filename, SpriteList* sprList);
void SpriteFreeDefinitions(SpriteList* sprList);
*/

class SpriteList
{
public:
    SpriteList(SDL_Renderer* renderer, const char* filename);
    ~SpriteList();
    int GetCount();
    int GetInitialised();
    Sprite* GetSprite(int index);

private:
    Sprite** list;
    int count;
    int initialised;
};
#endif // SPRITELOADER_H_INCLUDED
