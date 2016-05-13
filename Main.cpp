#include "Game.h"
#include "Globals.h"

#undef main

// Global sprite list
static SpriteList* sprites;

// main game
static Game* game = NULL;

int main(int argc, char** argv)
{
    game = new GameParaPara(argc, argv);

    // init the game
    if(game->GetInitialised())
    {
        // Create sprite list
        sprites = new SpriteList(game->GetMainRenderer(), "sprite_definitions.dat");
        SDL_SetTextureBlendMode(sprites->GetSprite(OBJ_SHOCKWAVE)->GetTexture(),
                                SDL_BLENDMODE_ADD);
        if(!sprites->GetInitialised())
            EHLog("main(): Cannot load sprite definitions.\n");
        else
        {
            game->MainLoop();
        }
    }

    delete game;
    delete sprites;
    return 0;
}

////////////////////////////////////////////////////////////////
// Get global sprite list
SpriteList* GetSpriteList()
{
    return sprites;
}
