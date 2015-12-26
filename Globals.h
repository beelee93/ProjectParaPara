//////////////////////////////////////////
// Globals.h
// Defines constants for resource ids
// Sprite ids should coincide with their
// object type ids
// Also defines inheritances
//////////////////////////////////////////
#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED

/////// Objects ////////
#define OBJ_DOWN_ARROW      0
#define OBJ_DEFAULT_ARROWS  1

#include "GameObjectManager.h"
#include "Game.h"
#include "SpriteLoader.h"

/////// Global Functions ////////
BL_SpriteList* GetSpriteList();

/////// Inherited Game ////////
class GameParaPara : public BL_Game
{
public:
    GameParaPara(int, char**);

    void OnUpdate(double secs);
    void OnEvent(SDL_Event* event, double secs);
};

/////// Inherited GOM ////////
class GOMParaPara : public BL_GOM
{
public:
    GOMParaPara();
    BL_GameObject* OnCreateObject(int type);
};

/////// Objects Classes ////////
class GODefaultArrow : public BL_GameObject
{
public:
    GODefaultArrow();
    void OnInit(int id, int type);
    void OnUpdate(double secs);
};

class GODownArrow : public BL_GameObject
{
public:
    GODownArrow();
    void OnInit(int id, int type);
};



#endif // RESOURCE_H_INCLUDED
