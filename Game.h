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

////////// Constants //////////
#define BL_IF_SDL       1
#define BL_IF_IMG       2
#define WINDOW_TITLE    "Para Para"
#define FPS_CAP         60

////////// Functions //////////

// Initialises the game
int BL_InitGame(int, char**);

// Initialises the window and its renderer
int BL_InitWindow(int fullscreen);

// Game loop
void BL_MainLoop();

// Rendering function
void BL_Render(double secs);

// Updating function
void BL_Update(double secs);

// Clean up
void BL_ExitGame();

// Sets the event listener for the game
// -void EventProc(SDL_Event* event, double seconds)
void BL_SetEventProcessor(void (*pfnEvtProc)(SDL_Event*,double));

// The function will be called twice every update cycle
// -Create Update(int pre,double secs)
// -Pre-GOM-update -> pre=1
// -Post-GOM-Update -> pre=0
void BL_SetUpdateFunction(void (*pfnUpdater)(int,double));

// Signals to the main game loop to stop
void BL_SignalExit();

// Sets whether to display on fullscreen
// -1 for toggle
void BL_SetFullscreen(int fscr);



BL_GOM* BL_GetGOM();
SDL_Renderer* BL_GetMainRenderer();
SDL_Window* BL_GetMainWindow();

#endif // GAME_H_INCLUDED
