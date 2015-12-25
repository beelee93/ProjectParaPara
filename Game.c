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
static uint32_t prevClock = 0;

static BL_Sprite sprArrow;
static BL_GameObject objNull;

////////// Functions //////////
void ProcessCmdLine(int, char**);

// Main Entry Point
int main(int argc, char** argv)
{
    // Process cmd line
    ProcessCmdLine(argc, argv);

    // Initialise error logger
    BL_EHInit();

    // Initialise the game
    if(BL_InitGame())
    {
        // Set clock
        prevClock = SDL_GetTicks();

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

    // Test sprite and object
    if(!BL_SpriteCreate(mainRenderer, "res/down_arrow.png", &sprArrow, 1))
        return 0;

    BL_SpriteSetRect(&sprArrow, 0, 0,0,256,256);

    BL_ObjectInit(&objNull, OBJ_NULL);
    objNull.sprite = &sprArrow;
    objNull.vx = objNull.vy = 5;
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

    if(isFullscreen)
    {
        if(SDL_SetWindowFullscreen(mainWindow, SDL_WINDOW_FULLSCREEN)!=0)
        {
            isFullscreen=0;
            BL_EHLog("InitWindow(): Selected fullscreen mode not supported. Reverting to"
                     "windowed mode.\n");
        }
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
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:   // end game
                            isGameLooping=0;
                            break;
                        case SDLK_F2:       // toggle fullscreen
                            isFullscreen = !isFullscreen;
                            SDL_SetWindowFullscreen(mainWindow,
                                isFullscreen ? SDL_WINDOW_FULLSCREEN : 0);
                            break;
                    }
            }
        }

        // Calculate elapsed time from last update
        curClock = SDL_GetTicks();
        secs = (curClock - prevClock) / 1000.0;
        prevClock = curClock;

        // Update
        BL_Update(secs);

        // we now time the rendering
        curClock = SDL_GetTicks();

        // Draw things
        BL_Render(secs);

        curClock = SDL_GetTicks() - curClock;
        secs = 1000.0 / FPS_CAP - curClock;
        if(secs>0)
            SDL_Delay( (int)secs );
    }
}

////////////////////////////////////////////////////////////////
// Update game stuff
void BL_Update(double secs)
{
    // DEBUG: display fps on window title
    char msg[256] = "";
    sprintf(msg, "FPS %f", 1/secs);
    SDL_SetWindowTitle(mainWindow, msg);

    // Test object update
    BL_ObjectUpdate(&objNull, secs);
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

    // Test object render
    BL_ObjectRender(&objNull,secs,mainRenderer);

    SDL_RenderPresent(mainRenderer);
}


////////////////////////////////////////////////////////////////
// Clean up
void BL_ExitGame()
{
    // Free resources
    BL_SpriteDestroy(&sprArrow);

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
// Process command line options
void ProcessCmdLine(int argc, char** argv)
{
    int i;
    for(i=1;i<argc;i++)
    {
        if(strcmp(argv[i], "--fullscreen")==0)
            isFullscreen = SDL_WINDOW_FULLSCREEN;
    }
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
