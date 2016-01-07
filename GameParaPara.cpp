//////////////////////////////////////////
// GameParaPara.cpp
// Inherits the Game class for the para para
// game
//////////////////////////////////////////

#include "Globals.h"

#ifdef WIN32
#define SYSFONT "C:/Windows/Fonts/Arial.ttf"
#else
#define SYSFONT "/usr/share/fonts/truetype/freefont/FreeMono.ttf"
#endif

static BL_GOM* gom = NULL;
static FC_Font* statusFont = NULL;

static double timer = 0;

// Test data for arrows
typedef struct __data {
    double timestamp;
    uint16_t arrow;
} Data;

Data data[1024]= {0};
static int dataIndex = 0;
static int dataCount = 0;
static uint16_t prevKeys = 0;
static double accuracy = 1;
static GOStationaryArrow* bwArrows[5];

///////////////////////////////////////////////////////////////////
GameParaPara::GameParaPara(int argc, char** argv) : BL_Game(argc, argv)
{
    // set initial game state
    gameState = GS_Splash;
    fadeTimer = 0.0;
    fadeMode = 0;
    fadeTarget = GS_Null;
	currInput = prevInput = 0;

    // initialise gom
    gom = new GOMParaPara();
    this->SetObjectManager(gom);

    ChangeGameState(GS_Splash);

    // create fonts
    statusFont = FC_CreateFont();

    if(!statusFont)
    {
        this->initialised = 0;
        BL_EHLog("GameParaPara(): Failed to create font.\n");
        return;
    }

    // use a system installed font
    FC_LoadFont(statusFont, mainRenderer, SYSFONT,
         16, FC_MakeColor(255,255,255,255), TTF_STYLE_NORMAL);
}

///////////////////////////////////////////////////////////////////
GameParaPara::~GameParaPara()
{
    // free GOM
    if(gom) delete gom;

    // free fonts
    if(statusFont) FC_FreeFont(statusFont);
}

///////////////////////////////////////////////////////////////////
void GameParaPara::OnUpdate(double secs)
{
    BL_Game::OnUpdate(secs);
	uint8_t riseKey = 0, fallKey = 0;
	SDL_Rect tempRect = {0};
    BL_GameObject* tempObj;

    switch(gameState)
    {
    //////////// UPDATE FOR SPLASHSCREEN ////////////
    case GS_Splash:
        timer -= secs;
        if(timer<=0)
        {
            gameState = GS_MainMenu;
            timer = 0;
        }
        break;

    //////////// UPDATE FOR ARENA ////////////
    case GS_Arena:
        int tempArenaX, tempArenaY;
        if(!arenaStarted)
        {
            // Preparations
            timer -= secs;
            if(timer<=0)
            {
                arenaStarted = 1;

                // play arena song
                timer = 0;
                dataIndex = 0;

                for(tempArenaX=0;tempArenaX<5;tempArenaX++)
                {
                    bwArrows[tempArenaX] = (GOStationaryArrow*)objManager->CreateObject(
                                            OBJ_DEFAULT_ARROWS_BW, &tempArenaX);
                    tempObj = objManager->CreateObject(OBJ_PINK_FLASH, &tempArenaX);
                    bwArrows[tempArenaX]->SetAttachedFlash((GOPinkFlash*)tempObj);
                }

            }
        }
        else
        {
            // Playing time!
            timer+=secs;

            while(timer>=(data[dataIndex].timestamp - FlightTime) && dataIndex < dataCount)
            {
                for(tempArenaX = 0;tempArenaX<5;tempArenaX++)
                {
                    if(data[dataIndex].arrow & (0x1 << (tempArenaX*2)))
                    {
                        objManager->CreateObject(OBJ_DEFAULT_ARROWS, &tempArenaX);
                    }
                }
                dataIndex++;
            }

			// Poll input
			PollInput();
			riseKey = ~prevInput & currInput; // look for rising edge
			fallKey = prevInput & ~currInput; // look for falling edge

			// look for flying arrows
			tempRect.x = 208;
			tempRect.y = 90;
			tempRect.w = 384;
			tempRect.h = 2;
			BL_GameObject** nearObjs = (BL_GameObject**)objManager->FindObjectsOfType(
				OBJ_DEFAULT_ARROWS, &tempRect);
			
			for (tempArenaX = 0; tempArenaX < 5; tempArenaX++) // loop through each arrow
			{
				if (currInput & (1 << tempArenaX))
				{
					// we flash the corresponding arrow
					bwArrows[tempArenaX]->Flash();
				}

				// is it a rising or falling edge?
				if (riseKey)
				{
					// we look for singular arrows and beginning of arrow chain
					// loop through all flying arrows detected
					for (tempArenaY = 0; nearObjs[tempArenaY] != NULL; tempArenaY++)
					{
						tempObj = nearObjs[tempArenaY];
						if (tempArenaX == tempObj->imageIndex && tempObj->alpha > 0.9)
						{
							accuracy += 1 - (
								(tempObj->y >64 ? tempObj->y : 64) - 64) / 64.0;
							// make it disappear
							((GODefaultArrow*)tempObj)->Disappear();
						}
					}
				}
				else if (fallKey)
				{
					// we look for end of arrow chain
				}		
			}
        }
        break;

    //////////// UPDATE FOR EDITOR ////////////
    case GS_Editor:
        timer+=secs;

        // get current keys
		PollInput();

		riseKey = ~prevInput & currInput;
		fallKey = prevInput & ~currInput;

        if( riseKey ) // check for rising edge
        {
            data[dataIndex].timestamp = timer - 0.1;
			data[dataIndex].arrow = (riseKey & 0x1) |
				((riseKey & 0x2) << 1) |
				((riseKey & 0x4) << 2) |
				((riseKey & 0x8) << 3) |
				((riseKey & 0x10) << 4);
            dataIndex++;
        }

        if(dataIndex>1023) dataIndex=1023;
        break;
    }

    // Transitional fade mechanism
    if(fadeMode)
    {
        if(fadeMode == FM_FADE_OUT)
        {
            fadeTimer += secs;
            if(fadeTimer>1.0) // we have occluded the entire screen
            {
                fadeTimer = 1.0;
                ChangeGameState(fadeTarget);
                fadeMode = FM_FADE_IN;
            }
        }
        else if(fadeMode == FM_FADE_IN)
        {
            fadeTimer -= secs;
            if(fadeTimer<0) // occlusion has disappeared
            {
                fadeTimer = 0;
                fadeMode = FM_FADE_STOP;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////
void GameParaPara::OnRender(SDL_Renderer* renderer, double secs)
{
    switch(gameState)
    {
    case GS_Arena:
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
        if(objManager) objManager->Render(secs);
        FC_Draw(statusFont, renderer, 3,48, "Song time: %f\nAvg Accuracy: %f",
            timer, accuracy);

        break;

    default:
        SDL_SetRenderDrawColor(renderer,0x64,0x95,0xED,255);
		SDL_RenderClear(renderer);
        if(objManager) objManager->Render(secs);
        break;
    }


    // Transition fade out mechanism
    if(fadeMode)
    {
        SDL_SetRenderDrawColor(renderer,0,0,0,(int)(fadeTimer * 255));
        SDL_RenderFillRect(renderer, NULL);
    }

    FC_Draw(statusFont, renderer, 3,3, "FPS: %d\nGameState: %d",
            (int)(1/secs), gameState);

    SDL_RenderPresent(renderer);
}

///////////////////////////////////////////////////////////////////
void GameParaPara::PollInput()
{
	prevInput = currInput;
#ifdef USE_KEYBOARD_INPUT
	// Keyboard
	const uint8_t* keys = SDL_GetKeyboardState(NULL);
	
	currInput = (
		keys[SDL_SCANCODE_Y]      |
		keys[SDL_SCANCODE_U] << 1 |
		keys[SDL_SCANCODE_I] << 2 |
		keys[SDL_SCANCODE_O] << 3 |
		keys[SDL_SCANCODE_P] << 4);
#endif

#ifdef USE_GPIO_INPUT
	// Look at GPIO pins
#endif
}

///////////////////////////////////////////////////////////////////
void GameParaPara::OnEvent(SDL_Event* event, double secs)
{
    BL_Game::OnEvent(event, secs);

    switch(event->type)
    {
        ///// Keyup Event /////
         case SDL_KEYUP:
            switch(event->key.keysym.sym)
            {
                case SDLK_ESCAPE:   // end game
                    if(gameState == GS_MainMenu)
                        SignalExit();
                    else if(gameState == GS_Arena)
                        FadeToGameState(GS_MainMenu);
                    break;
                case SDLK_a:
                    GetObjectManager()->CreateObject(OBJ_DEFAULT_ARROWS);
                    break;

            }
            break;

        ///// Keydown Event /////
        case SDL_KEYDOWN:
            switch(event->key.keysym.sym)
            {
            case SDLK_RETURN:
                if(gameState == GS_MainMenu)
                {
                    // Transition to SongSelection
                    FadeToGameState(/*GS_SongSelection*/ GS_Arena);
                }
                break;
            case SDLK_F10:
                if(gameState == GS_MainMenu)
                {
                    // Transition to editor
                    FadeToGameState(GS_Editor);
                }
                else if(gameState == GS_Editor)
                {
                    // Transition back to Main menu
                    FadeToGameState(GS_MainMenu);
                }
                break;
            }
            break;
     }
}

///////////////////////////////////////////////////////////////////
// Raised upon leaving a game state. Argument is the state we
// are leaving
void GameParaPara::OnExitState(GameState leavingState)
{
    switch(leavingState)
    {
    case GS_Arena:

        break;
    case GS_Editor:
        dataCount = dataIndex;
        break;
    }

    // remove all objects in scene
    objManager->DestroyAllObjects();
}

///////////////////////////////////////////////////////////////////
// Raised upon entering a new game state. Argument is the
// current state
void GameParaPara::OnEnterState(GameState newState)
{
    switch(newState)
    {
    case GS_Splash:
        timer = 2.0;
        break;
    case GS_Arena:
        timer = 2.0;
        arenaStarted = 0;
		prevInput = currInput = 0;
        break;
    case GS_Editor:
        timer=0.0;
        dataIndex=0;
		prevInput = currInput = 0;
        break;
    }
}

///////////////////////////////////////////////////////////////////
void GameParaPara::ChangeGameState(GameState newState)
{
    OnExitState(gameState);
    gameState = newState;
    OnEnterState(gameState);
}

///////////////////////////////////////////////////////////////////
// Transitional fade mechanism
void GameParaPara::FadeToGameState(GameState state)
{
    if(fadeTarget != state)
    {
        fadeMode = FM_FADE_OUT; // fade out
        fadeTarget = state;
    }
}
