#include "Game.h"
#include "Resource.h"

// Test sprite list
static BL_SpriteList sprites;
static BL_GOM *gom;

// Custom loaders
BL_Sprite* IdToSprite(int resId);
void ObjectInitialiser(BL_GameObject* obj, int type);
void EventProc(SDL_Event* event, double secs);
void ObjectUpdater(BL_GameObject*,double);
void UpdateFunc(int, double);

// timer
static double countdown = 2.0;

int main(int argc, char** argv)
{
    // Set the loaders
    BL_SetObjectSpriteLoader(IdToSprite);
    BL_SetObjectInitialiser(ObjectInitialiser);
    BL_SetEventProcessor(EventProc);
    BL_SetObjectUpdater(ObjectUpdater);
    BL_SetUpdateFunction(UpdateFunc);

    // init the game
    if(BL_InitGame(argc, argv))
    {
        // Create sprite list
        gom = BL_GetGOM();
        if(!BL_SpriteLoadDefinitions(BL_GetMainRenderer(), "sprite_definitions.dat", &sprites))
            BL_EHLog("main(): Cannot load sprite definitions.\n");
        else
            BL_MainLoop();
    }

    BL_ExitGame();

    return 0;
}

////////////////////////////////////////////////////////////////
// Update function
void UpdateFunc(int pre, double secs)
{
    if(!pre) // Post-GOM update
    {
        countdown-=secs;
        if(countdown<=0)
        {
            countdown=(rand()%5+1) * 0.2;
            BL_GOMCreateObject(gom, OBJ_DEFAULT_ARROWS);
        }
    }
}

////////////////////////////////////////////////////////////////
// Sprite loader function
BL_Sprite* IdToSprite(int resId)
{
    return &(sprites.list[resId]);
}

////////////////////////////////////////////////////////////////
// Object initialiser function
void ObjectInitialiser(BL_GameObject* obj, int type)
{
    switch(type)
    {
    case OBJ_DOWN_ARROW:
        break;
    case OBJ_DEFAULT_ARROWS:
        obj->imageSpeed=0;
        obj->imageIndex = rand() % 5;
        obj->x = 208 + 80*obj->imageIndex;
        obj->y = 600;
        obj->vy= -200;
        break;
    }
}

////////////////////////////////////////////////////////////////
// Event listener
void EventProc(SDL_Event* event, double secs)
{
    switch(event->type)
    {
        ///// Keyup Event /////
         case SDL_KEYUP:
            switch(event->key.keysym.sym)
            {
                case SDLK_ESCAPE:   // end game
                    BL_SignalExit();
                    break;
                case SDLK_F2:       // toggle fullscreen
                    BL_SetFullscreen(-1);
                    break;
                case SDLK_a:
                    BL_GOMCreateObject(gom, OBJ_DEFAULT_ARROWS);
                    break;
            }
     }
}

////////////////////////////////////////////////////////////////
// Object update
void ObjectUpdater(BL_GameObject* obj,double secs)
{
    switch(obj->type)
    {
    case OBJ_DEFAULT_ARROWS:
        if(obj->y<-64)
            BL_GOMDestroyObjectById(gom, obj->id);
    break;
    }
}
