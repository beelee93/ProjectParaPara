//////////////////////////////////////////
// Game.c
// Implementation for the game foundations
//////////////////////////////////////////

#include "Game.h"

// Initialises the game
Game::Game(int argc, char** argv)
{
    initFlags = 0;
	isFullscreen = 0;
    isGameLooping = 1;
    windowWidth = 800;
    windowHeight = 600;
    objManager = NULL;
    mainWindow = NULL;
    mainRenderer = NULL;
	audio = NULL;

    // Process cmd line
    ProcessCmdLine(argc, argv);

    // Initialise error logger
    EHInit();

    // Initialise SDL
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0)
    {
        EHLog("InitGame(): Could not initialise SDL.\n");
        goto err;
    }

    // Set flag
    initFlags |= IF_SDL;

    // Initialise IMG
    if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0)
    {
        EHLog("InitGame(): Could not initialise IMG.\n");
        goto err;
    }

    // Set flag
    initFlags |= IF_IMG;

     // Initialise TTF
    if(TTF_Init() != 0)
    {
        EHLog("InitGame(): Could not initialise TTF.\n");
        goto err;
    }

    // Set flag
    initFlags |= IF_TTF;

    if(!InitWindow(isFullscreen))
    {
        EHLog("InitGame(): Could not initialise window or renderer.\n");
        goto err;
    }

	// Initialise audio
	audio = new Audio();
	if (!audio || !audio->GetInitialised())
	{
		EHLog("InitGame(): Could not initialise audio system.\n");
		goto err;
	}

    // Set clock
    prevClock = SDL_GetTicks();

    initialised = 1;

    return;
    err:
        EHLog("RunGame(): Could not initialise game.\n");
        initialised = 0;
}

// frees the game
Game::~Game()
{
    if(mainRenderer)
        SDL_DestroyRenderer(mainRenderer);
    if(mainWindow)
        SDL_DestroyWindow(mainWindow);
	if (audio)
		delete audio;

    // Free libraries
    if(initFlags & IF_TTF)
        TTF_Quit();
    if(initFlags & IF_IMG)
        IMG_Quit();
    if(initFlags & IF_SDL)
        SDL_Quit();

    // Save outputs
    EHFlush();
    EHQuit();
}

// game's main loop
void Game::MainLoop()
{
    uint32_t curClock;
    SDL_Event event;
    double secs;

    while(isGameLooping)
    {
        // Calculate elapsed time from last update
        curClock = SDL_GetTicks();
        secs = (curClock - prevClock) / 1000.0;
        prevClock = curClock;

        // Poll for events
        while(SDL_PollEvent(&event))
        {
            OnEvent(&event, secs);
        }

        // Update
        OnUpdate(secs);

        // we now time the rendering
        curClock = SDL_GetTicks();

        // Draw things
        OnRender(mainRenderer, secs);

        curClock = SDL_GetTicks() - curClock;
        secs = 1000.0 / FPS_CAP - curClock;
        if(secs>0)
            SDL_Delay( (int)secs );
    }
}

// Called on every update cycle
void Game::OnUpdate(double secs)
{
	if (secs > 2.0f) return;

    if(objManager) objManager->Update(secs);
}

// Called on every render cycle
void Game::OnRender(SDL_Renderer* renderer,double secs)
{
	if (secs > 2.0f) return;
    SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
    SDL_RenderClear(renderer);

    // Render GOM
    if(objManager) objManager->Render(secs);

    SDL_RenderPresent(renderer);
}

void Game::OnEvent(SDL_Event* event, double secs)
{
     switch(event->type)
     {
        ///// Window Event /////
        case SDL_WINDOWEVENT:
            switch(event->window.event)
            {
                  case SDL_WINDOWEVENT_CLOSE:
                    // signal to terminate
                    isGameLooping = 0;
                    break;
            }
        break;
    }
}

// Initialises the game window
int Game::InitWindow(int fullscreen)
{
    if(mainWindow)
    {
         EHLog("InitWindow(): Main window has not been released.\n");
         return 0;
    }

    mainWindow = SDL_CreateWindow(WINDOW_TITLE, 45,45,
        windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if(!mainWindow)
    {
        EHLog("InitWindow(): Could not create window.\n");
        return 0;
    }

    if(isFullscreen)
    {
        if(SDL_SetWindowFullscreen(mainWindow, SDL_WINDOW_FULLSCREEN)!=0)
        {
            isFullscreen=0;
            EHLog("InitWindow(): Selected fullscreen mode not supported. Reverting to"
                     "windowed mode.\n");
        }
    }

    mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
    if(!mainRenderer)
    {
        EHLog("InitWindow(): Could not create renderer.\n");
        return 0;
    }

    SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_BLEND);
    return 1;
}

// Signal the main game loop to stop
void Game::SignalExit()
{
    isGameLooping = 0;
}

// Set fullscreen/window/toggle fullscreen
void Game::SetFullscreen(SET_FULLSCREEN fscr)
{
    if(fscr==SF_WINDOWED_MODE)
        isFullscreen = 0;
    else if(fscr==SF_FULLSCREEN_MODE)
        isFullscreen = 1;
    else
        isFullscreen = !isFullscreen;
    SDL_SetWindowFullscreen(mainWindow,
        isFullscreen ? SDL_WINDOW_FULLSCREEN : 0);
}

void Game::ProcessCmdLine(int argc, char** argv)
{
    int i;
    for(i=1;i<argc;i++)
    {
        if(strcmp(argv[i], "--fullscreen")==0)
            isFullscreen = SDL_WINDOW_FULLSCREEN;
    }
}

Audio* Game::GetAudio()
{
	return audio;
}

SDL_Renderer* Game::GetMainRenderer()
{
    return mainRenderer;
}
SDL_Window* Game::GetMainWindow()
{
    return mainWindow;
}

GOM* Game::GetObjectManager()
{
    return objManager;
}

void Game::SetObjectManager(GOM* mgr)
{
    objManager = mgr;
    objManager->SetAttachedRenderer(mainRenderer);
}

int Game::GetInitialised()
{
    return initialised;
}
