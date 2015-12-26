//////////////////////////////////////////
// GameParaPara.cpp
// Inherits the Game class for the para para
// game
//////////////////////////////////////////

#include "Globals.h"


// timer
static double countdown = 2.0;

GameParaPara::GameParaPara(int argc, char** argv) : BL_Game(argc, argv) { }

void GameParaPara::OnUpdate(double secs)
{
    BL_Game::OnUpdate(secs);

    countdown-=secs;
    if(countdown<=0)
    {
        countdown=(rand()%5+1) * 0.2;
        GetObjectManager()->CreateObject(OBJ_DEFAULT_ARROWS);
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
            }
     }
}
