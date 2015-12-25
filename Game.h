//////////////////////////////////////////
// Game.h
// Defines function prototypes needed to
// run the main game
//////////////////////////////////////////

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

// Include our own error logger
// which also has stdio.h included
#include "ErrorHandler.h"

// Include sprite sheet functions
#include "Sprite.h"
#include "GameObject.h"

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
int BL_InitGame();

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

// Access the main window
SDL_Window* BL_GetMainWindow();

// Access the main renderer
SDL_Renderer* BL_GetMainRenderer();

#endif // GAME_H_INCLUDED
