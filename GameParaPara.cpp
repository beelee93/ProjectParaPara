//////////////////////////////////////////
// GameParaPara.cpp
// Inherits the Game class for the para para
// game
//////////////////////////////////////////

#include "Globals.h"
// sound control
using namespace irrklang;
static BL_Audio* audio = NULL;

static BL_GOM* gom = NULL;
static FC_Font* statusFont = NULL;

static double timer = 0;
static ISound* music = NULL;

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

    // initialise gom
    gom = new GOMParaPara();
    this->SetObjectManager(gom);

    ChangeGameState(GS_Splash);

    // initialise audio system
    audio = new BL_Audio();
    if(!audio)
    {
        this->initialised = 0;
        BL_EHLog("GameParaPara(): Failed to create audio controller.\n");
        return;
    }

    // load audio
    audio->AddAudio("res/Yesterday.mp3", 1);

    // create fonts
    statusFont = FC_CreateFont();

    if(!statusFont)
    {
        this->initialised = 0;
        BL_EHLog("GameParaPara(): Failed to create font.\n");
        return;
    }

    // use a system installed font
    FC_LoadFont(statusFont, mainRenderer, "/usr/share/fonts/truetype/freefont/FreeMono.ttf",
         16, FC_MakeColor(255,255,255,255), TTF_STYLE_NORMAL);
}

///////////////////////////////////////////////////////////////////
GameParaPara::~GameParaPara()
{
    // free GOM
    if(gom) delete gom;

    // free audio
    if(audio) delete audio;

    // free fonts
    if(statusFont) FC_FreeFont(statusFont);
}

///////////////////////////////////////////////////////////////////
void GameParaPara::OnUpdate(double secs)
{
    BL_Game::OnUpdate(secs);
    const uint8_t* keys = SDL_GetKeyboardState(NULL);
    uint16_t currKeys = 0, checkedKeys = 0;
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
                music = audio->PlayAudio(0, 0);
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
                        tempArenaY = 4-tempArenaX;
                        objManager->CreateObject(OBJ_DEFAULT_ARROWS, &tempArenaY);
                    }
                }
                dataIndex++;
            }

            // get current keys
            currKeys = (
                keys[SDL_SCANCODE_Y]<<8 |
                keys[SDL_SCANCODE_U]<<6 |
                keys[SDL_SCANCODE_I]<<4 |
                keys[SDL_SCANCODE_O]<<2 |
                keys[SDL_SCANCODE_P] );
            checkedKeys = ~prevKeys & currKeys;
            if( checkedKeys ) // check for rising edge
            {
                SDL_Rect tempRect;
                tempRect.x = 208;
                tempRect.y = 90;
                tempRect.w = 384;
                tempRect.h = 2;

                BL_GameObject** nearObjs = (BL_GameObject**)objManager->FindObjectsOfType(
                                            OBJ_DEFAULT_ARROWS, &tempRect);
                for(tempArenaX=0;tempArenaX<5;tempArenaX++)
                {
                    if(checkedKeys & (0x1 << (tempArenaX*2)))
                    {
                        tempArenaY = 4-tempArenaX;
                        bwArrows[tempArenaY]->Flash();

                        for(tempArenaY=0;nearObjs[tempArenaY]!=NULL;tempArenaY++)
                        {
                            if(nearObjs[tempArenaY]->imageIndex == tempArenaX &&
                                nearObjs[tempArenaY]->alpha > 0.9)
                            {
                                accuracy += 1-(
                                 (nearObjs[tempArenaY]->y >64 ? nearObjs[tempArenaY]->y:64)-64)/64.0;
                                accuracy /= 2;
                                ((GODefaultArrow*)nearObjs[tempArenaY])->Disappear();
                                break;
                            }
                        }
                    }
                }
            }
            prevKeys = currKeys;
        }
        break;

    //////////// UPDATE FOR EDITOR ////////////
    case GS_Editor:
        timer+=secs;

        // get current keys
        currKeys = (
            keys[SDL_SCANCODE_Y]<<8 |
            keys[SDL_SCANCODE_U]<<6 |
            keys[SDL_SCANCODE_I]<<4 |
            keys[SDL_SCANCODE_O]<<2 |
            keys[SDL_SCANCODE_P] );

        checkedKeys = ~prevKeys & currKeys;
        if( checkedKeys ) // check for rising edge
        {
            data[dataIndex].timestamp = timer - 0.1;
            data[dataIndex].arrow = checkedKeys;
            dataIndex++;
        }

        if(dataIndex>1023) dataIndex=1023;
        prevKeys = currKeys;
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
                case SDLK_F2:       // toggle fullscreen
                    SetFullscreen(SF_TOGGLE);
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
            case SDLK_F12:
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
        if(music)
        {
            music->stop();
            music->drop();
            music = NULL;
        }
        break;
    case GS_Editor:
        if(music)
        {
            music->stop();
            music->drop();
            music = NULL;
        }
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
        break;
    case GS_Editor:
        timer=0.0;
        dataIndex=0;
        music = audio->PlayAudio(0,0);
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
