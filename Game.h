//////////////////////////////////////////
// Game.h
// Defines function prototypes needed to
// run the main game
//////////////////////////////////////////

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "ErrorHandler.h"
#include "Sprite.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "SpriteLoader.h"

#include <string.h>
#include <stddef.h>

// SDL Includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

////////// Constants //////////
#define BL_IF_SDL       1
#define BL_IF_IMG       2
#define BL_IF_TTF       4

#define WINDOW_TITLE    "Para Para"
#define FPS_CAP         60

enum BL_SET_FULLSCREEN {
    SF_WINDOWED_MODE,
    SF_FULLSCREEN_MODE,
    SF_TOGGLE
};

class BL_Game
{
public:
    BL_Game(int argc, char** argv);
    ~BL_Game();

    void MainLoop();

    virtual void OnUpdate(double secs);
    virtual void OnRender(SDL_Renderer* renderer,double secs);
    virtual void OnEvent(SDL_Event* event, double secs);

    void SignalExit();
    void SetFullscreen(BL_SET_FULLSCREEN fscr);
    SDL_Renderer* GetMainRenderer();
    SDL_Window* GetMainWindow();
    BL_GOM* GetObjectManager();
    int GetInitialised();
    void SetObjectManager(BL_GOM* gom);

protected:
    SDL_Renderer* mainRenderer;
    SDL_Window* mainWindow;
    BL_GOM* objManager;

    int initialised;
    int isFullscreen;
    int windowWidth;
    int windowHeight;
    int InitWindow(int fullscreen);


private:
    void ProcessCmdLine(int argc, char** argv);
    int initFlags;
    int isGameLooping;
    uint32_t prevClock;
};

#endif // GAME_H_INCLUDED
