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
#include "Audio.h"

#include <string.h>
#include <stddef.h>

// SDL Includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

////////// Constants //////////
#define IF_SDL       1
#define IF_IMG       2
#define IF_TTF       4

#define WINDOW_TITLE    "Para Para"
#define FPS_CAP         60

enum SET_FULLSCREEN {
    SF_WINDOWED_MODE,
    SF_FULLSCREEN_MODE,
    SF_TOGGLE
};

class Game
{
public:
    Game(int argc, char** argv);
    virtual ~Game();

    void MainLoop();

    virtual void OnUpdate(double secs);
    virtual void OnRender(SDL_Renderer* renderer,double secs);
    virtual void OnEvent(SDL_Event* event, double secs);

    void SignalExit();
    void SetFullscreen(SET_FULLSCREEN fscr);
    SDL_Renderer* GetMainRenderer();
    SDL_Window* GetMainWindow();
    GOM* GetObjectManager();
    int GetInitialised();
    void SetObjectManager(GOM* gom);

	Audio* GetAudio();

protected:
    SDL_Renderer* mainRenderer;
    SDL_Window* mainWindow;
    GOM* objManager;
	Audio* audio;

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
