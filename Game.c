//////////////////////////////////////////
// Game.c
// Implementation for the para para game
//////////////////////////////////////////

#include "Game.h"

////////// Variables //////////
static int initFlags = 0;
static SDL_Window* mainWindow = NULL;
static SDL_Renderer* mainRenderer = NULL;
static int isFullscreen = 0;
static uint32_t windowWidth = 800, windowHeight = 600;
static int isGameLooping = 1;
static clock_t prevClock = 0;

////////// Functions //////////

// Main Entry Point
int main(int argc, char** argv)
{
    // Initialise error logger
    BL_EHInit();

    // Initialise the game
    if(BL_InitGame())
    {
        // Set clock
        prevClock = clock();

        // Call game loop
        BL_MainLoop();
    }
    else
    {
        BL_EHLog("main(): Could not initialise game. Terminating.\n");
    }


    // End game, but first, we clean up
    BL_ExitGame();

    // Save outputs
    BL_EHFlush();
    BL_EHQuit();
    return 0;
}

////////////////////////////////////////////////////////////////
// Initialises the game
int BL_InitGame()
{
    // Initialise SDL
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0)
    {
        BL_EHLog("InitGame(): Could not initialise SDL.\n");
        return 0;
    }

    // Set flag
    initFlags |= BL_IF_SDL;

    // Initialise IMG
    if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0)
    {
        BL_EHLog("InitGame(): Could not initialise IMG.\n");
        return 0;
    }

    // Set flag
    initFlags |= BL_IF_IMG;

    if(!BL_InitWindow(isFullscreen))
    {
        BL_EHLog("InitGame(): Could not initialise window or renderer.\n");
        return 0;
    }
    return 1;
}

////////////////////////////////////////////////////////////////
int BL_InitWindow(int fullscreen)
{
    if(mainWindow)
    {
         BL_EHLog("InitWindow(): Main window has not been released.\n");
         return 0;
    }

    // TODO: Implement fullscreen capabilities
    mainWindow = SDL_CreateWindow(WINDOW_TITLE, 0,0,
        windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if(!mainWindow)
    {
        BL_EHLog("InitWindow(): Could not create window.\n");
        return 0;
    }

    mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
    if(!mainRenderer)
    {
        BL_EHLog("InitWindow(): Could not create renderer.\n");
        return 0;
    }

    return 1;
}

////////////////////////////////////////////////////////////////
// Main game loop
void BL_MainLoop()
{
    clock_t curClock;
    SDL_Event event;
    double secs;

    while(isGameLooping)
    {
        // Poll for events
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                ///// Window Event /////
                case SDL_WINDOWEVENT:
                    switch(event.window.event)
                    {
                        case SDL_WINDOWEVENT_CLOSE:
                            // signal to terminate
                            isGameLooping = 0;
                            break;
                    }
                    break;

                 ///// Keyup Event /////
                 case SDL_KEYUP:
                            if(event.key.keysym.sym == SDLK_ESCAPE)
                                isGameLooping = 0;
                            break;
            }
        }

        // Calculate elapsed time from last update
        curClock = clock();
        secs = (curClock - prevClock) / (double)CLOCKS_PER_SEC;
        prevClock = curClock;


        // Update
        BL_Update(secs);

        // we now time the rendering
        curClock = clock();

        // Draw things
        BL_Render(secs);

        curClock = clock() - curClock;
        secs = 1000.0 / FPS_CAP - curClock * 1000.0 / CLOCKS_PER_SEC;
        if(secs>0)
            SDL_Delay( (int)secs );
    }
}

////////////////////////////////////////////////////////////////
// Update game stuff
void BL_Update(double secs)
{

}

////////////////////////////////////////////////////////////////
// Render things on screen
void BL_Render(double secs)
{
    SDL_Rect rect;
    rect.x = 10;
    rect.y = 10;
    rect.w = windowWidth-20;
    rect.h = windowHeight-20;

    SDL_SetRenderDrawColor(mainRenderer, 0x64, 0x95, 0xED, 255);
    SDL_RenderClear(mainRenderer);

    SDL_SetRenderDrawColor(mainRenderer, 0,0,0, 255);
    SDL_RenderDrawRect(mainRenderer, &rect);

    SDL_RenderPresent(mainRenderer);
}


////////////////////////////////////////////////////////////////
// Clean up
void BL_ExitGame()
{
    // Free resources
    if(mainRenderer)
        SDL_DestroyRenderer(mainRenderer);
    if(mainWindow)
        SDL_DestroyWindow(mainWindow);

    // Free libraries
    if(initFlags & BL_IF_IMG)
        IMG_Quit();
    if(initFlags & BL_IF_SDL)
        SDL_Quit();
}

////////////////////////////////////////////////////////////////
// Access main window
SDL_Window* BL_GetMainWindow()
{
    return mainWindow;
}

////////////////////////////////////////////////////////////////
// Acces main renderer
SDL_Renderer* BL_GetMainRenderer()
{
    return mainRenderer;
}
