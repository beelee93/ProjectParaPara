//////////////////////////////////////////
// Game.c
// Implementation for the game foundations
//////////////////////////////////////////

#include "Game.h"

////////// Variables //////////
static int initFlags = 0;
static SDL_Window* mainWindow = NULL;
static SDL_Renderer* mainRenderer = NULL;
static int isFullscreen = 0;
static uint32_t windowWidth = 800, windowHeight = 600;
static int isGameLooping = 1;
static uint8_t clearColor[3] = {0,0,0};
static uint32_t prevClock = 0;

static void (*pfnProcessEvent)(SDL_Event*,double) = NULL;
static void (*pfnUpdateFunc)(int,double) = NULL;

static BL_GOM objManager;

////////// Functions //////////
void ProcessCmdLine(int, char**);

////////////////////////////////////////////////////////////////
// Initialises the game
int BL_InitGame(int argc, char** argv)
{
    // Process cmd line
    ProcessCmdLine(argc, argv);

    // Initialise error logger
    BL_EHInit();

    // Initialise SDL
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0)
    {
        BL_EHLog("InitGame(): Could not initialise SDL.\n");
        goto err;
    }

    // Set flag
    initFlags |= BL_IF_SDL;

    // Initialise IMG
    if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0)
    {
        BL_EHLog("InitGame(): Could not initialise IMG.\n");
        goto err;
    }

    // Set flag
    initFlags |= BL_IF_IMG;

    if(!BL_InitWindow(isFullscreen))
    {
        BL_EHLog("InitGame(): Could not initialise window or renderer.\n");
        goto err;
    }

    // Set clock
    prevClock = SDL_GetTicks();

    return 1;

    err:
        BL_EHLog("RunGame(): Could not initialise game.\n");
        return 0;
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

    // initialise game object manager
    BL_GOMInit(&objManager);

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

                default:
                    if(pfnProcessEvent)
                        pfnProcessEvent(&event, secs);
                    break;
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
    sprintf(msg, "FPS %f GOM_COUNT %d GOM_CAP %d", 1/secs,objManager.objCount, objManager.capacity);
    SDL_SetWindowTitle(mainWindow, msg);

    if(pfnUpdateFunc) pfnUpdateFunc(1, secs);
    // update GOM
    BL_GOMUpdate(&objManager, secs);

    if(pfnUpdateFunc) pfnUpdateFunc(0, secs);
}

////////////////////////////////////////////////////////////////
// Render things on screen
void BL_Render(double secs)
{
    SDL_SetRenderDrawColor(mainRenderer, clearColor[0], clearColor[1], clearColor[2], 255);
    SDL_RenderClear(mainRenderer);

    // Render GOM
    BL_GOMRender(&objManager, mainRenderer, secs);

    SDL_RenderPresent(mainRenderer);
}


////////////////////////////////////////////////////////////////
// Clean up
void BL_ExitGame()
{
    // Free object manager
    BL_GOMFree(&objManager);

    if(mainRenderer)
        SDL_DestroyRenderer(mainRenderer);
    if(mainWindow)
        SDL_DestroyWindow(mainWindow);

    // Free libraries
    if(initFlags & BL_IF_IMG)
        IMG_Quit();
    if(initFlags & BL_IF_SDL)
        SDL_Quit();

    // Save outputs
    BL_EHFlush();
    BL_EHQuit();
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
// Signals to the main game loop to stop
void BL_SignalExit()
{
    isGameLooping = 0;
}

////////////////////////////////////////////////////////////////
// Sets whether to display on fullscreen
// -1 for toggle
void BL_SetFullscreen(int fscr)
{
    if(fscr<0)
        isFullscreen = !isFullscreen;
    else
        isFullscreen = fscr;
    SDL_SetWindowFullscreen(mainWindow,
        isFullscreen ? SDL_WINDOW_FULLSCREEN : 0);
}



////////////////////////////////////////////////////////////////
SDL_Renderer* BL_GetMainRenderer()
{
    return mainRenderer;
}

SDL_Window* BL_GetMainWindow()
{
    return mainWindow;
}

BL_GOM* BL_GetGOM()
{
    return &objManager;
}

////////////////////////////////////////////////////////////////
// Sets the event listener for the game
// -void EventProc(SDL_Event* event, double seconds)
void BL_SetEventProcessor(void (*pfnEvtProc)(SDL_Event*,double))
{
    pfnProcessEvent = pfnEvtProc;
}

////////////////////////////////////////////////////////////////
// The function will be called twice every update cycle
void BL_SetUpdateFunction(void (*pfnUpdater)(int,double))
{
    pfnUpdateFunc=pfnUpdater;
}
////////////////////////////////////////////////////////////////
// Sets the clear color
void BL_SetClearColor(uint8_t r,uint8_t g,uint8_t b)
{
    clearColor[0]=r;
    clearColor[1]=g;
    clearColor[2]=b;
}
