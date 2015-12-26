//////////////////////////////////////////
// GameParaPara.cpp
// Inherits the Game class for the para para
// game
//////////////////////////////////////////

#include "Globals.h"
#include "irrKlang.h"
#include "SDL_FontCache.h"

using namespace irrklang;
// timer
static double countdown = 2.0;
static ISoundEngine* soundEngine = NULL;
static ISoundSource* soundSrc = NULL;
static ISound* snd = NULL;

static FC_Font* statusFont = NULL;

static uint32_t songPos = 0;
static double songTimer = 0;
static uint32_t songLength = 0;

static double data[1024] = {0};
static int dataIndex = 0;
static int dataStop = 0;
static int filled = 0;

GameParaPara::GameParaPara(int argc, char** argv) : BL_Game(argc, argv)
{
    soundEngine = irrklang::createIrrKlangDevice();
    soundSrc=soundEngine->addSoundSourceFromFile("res/IntoYourHeart.mp3", ESM_AUTO_DETECT, true);

    statusFont = FC_CreateFont();

    // use a system installed font
    FC_LoadFont(statusFont, mainRenderer, "/usr/share/fonts/truetype/freefont/FreeMono.ttf",
           16, FC_MakeColor(255,255,255,255), TTF_STYLE_NORMAL);
}

GameParaPara::~GameParaPara()
{
    if(snd)
    {
        snd->stop();
        snd->drop();
    }

    FC_FreeFont(statusFont);

    if(soundEngine)soundEngine->drop();
}

void GameParaPara::OnUpdate(double secs)
{
    BL_Game::OnUpdate(secs);

    if(snd)
    {
        songTimer += secs;
        songPos = snd->getPlayPosition();

        if(filled && dataIndex<dataStop)
        {
             while(songTimer >= data[dataIndex]-2.85)
             {
                 if(dataIndex>=dataStop) break;
                 objManager->CreateObject(OBJ_DEFAULT_ARROWS);
                 dataIndex++;
             }
        }

    }
}


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
                    SignalExit();
                    break;
                case SDLK_F2:       // toggle fullscreen
                    SetFullscreen(SF_TOGGLE);
                    break;
                case SDLK_a:
                    GetObjectManager()->CreateObject(OBJ_DEFAULT_ARROWS);
                    break;
                case SDLK_b:
                    if(!snd)
                    {
                        snd = soundEngine->play2D(soundSrc, false, true, true);
                        songPos = 0;
                        songTimer = 0;
                        dataIndex = 0;
                        songLength = snd->getPlayLength();
                        if(snd)
                            snd->setIsPaused(false);
                    }
                    else
                    {
                        if(!filled)
                        {
                            dataStop = dataIndex;
                            filled=1;
                        }

                        snd->stop();
                        snd->drop();
                        snd=NULL;
                    }
                    break;


            }
            break;
        ///// Keydown Event /////
        case SDL_KEYDOWN:
            switch(event->key.keysym.sym)
            {
                case SDLK_y:
                    if(!filled)
                    {
                        data[dataIndex++] = songTimer - 0.18;
                        if(dataIndex>1023) dataIndex = 1023;
                    }
                    break;
            }
            break;
     }
}

void GameParaPara::OnRender(SDL_Renderer* renderer, double secs)
{
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);

    if(objManager) objManager->Render(secs);

    FC_Draw(statusFont, renderer, 3,3, "FPS: %d\nPos: %d/%d (%f)",
            (int)(1/secs), songPos,songLength, songTimer);

    SDL_RenderPresent(renderer);
}
