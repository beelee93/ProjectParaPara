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
#define OBJ_DEFAULT_ARROWS      0
#define OBJ_DEFAULT_ARROWS_BW   1
#define OBJ_SHOCKWAVE           2

#include "GameObjectManager.h"
#include "Game.h"
#include "SpriteLoader.h"
#include "irrKlang.h"
#include "SDL_FontCache.h"
#include "Audio.h"

/////// Constants ////////
enum GameState {
    GS_Null,                /* Used by fadeTarget */
    GS_Splash,              /* Splash screen */
    GS_MainMenu,            /* Press a button to begin */
    GS_SongSelection,       /* Menu to select songs */
    GS_Arena,               /* Main playing screen */
    GS_Scoreboard,          /* Results and score */
    GS_Editor               /* Allows the creation of new rhythms */
};

#define FM_FADE_OUT -1
#define FM_FADE_IN 1
#define FM_FADE_STOP 0


/////// Global Functions ////////
BL_SpriteList* GetSpriteList();

/////// Inherited Game ////////
class GameParaPara : public BL_Game
{
public:
    GameParaPara(int, char**);
    ~GameParaPara();
    void OnUpdate(double secs);
    void OnEvent(SDL_Event* event, double secs);
    void OnRender(SDL_Renderer* renderer, double secs);
    void OnExitState(GameState leavingState);
    void OnEnterState(GameState newState);
    void ChangeGameState(GameState newState);
    void FadeToGameState(GameState state);

protected:
    GameState gameState;
    double timer;

    // Transition fade mechanism
    double fadeTimer;
    int fadeMode;
    GameState fadeTarget;

    // Arena variables
    int arenaStarted;
};

/////// Inherited GOM ////////
class GOMParaPara : public BL_GOM
{
public:
    GOMParaPara();
    BL_GameObject* OnCreateObject(int type);
};

/////// Objects Classes ////////
#define ARROW_TARGET_Y  64
#define ARROW_SPEED     -200
#define FlightTime      ((600.0 - ARROW_TARGET_Y) / (-ARROW_SPEED))

class GODefaultArrow : public BL_GameObject
{
public:
    GODefaultArrow();
    void OnInit(int id, int type, void* data=NULL);
    void OnUpdate(double secs);
    void Disappear();
private:
    int disappearing;
};

class GOStationaryArrow : public BL_GameObject
{
public:
    GOStationaryArrow();
    void OnInit(int id, int type, void* data=NULL);
    void OnUpdate(double secs);
    void Flash();
};

#endif // RESOURCE_H_INCLUDED
