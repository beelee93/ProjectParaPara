#include "Game.h"
#include "Globals.h"

#undef main

// Global sprite list
static BL_SpriteList* sprites;

// main game
static BL_Game* game = NULL;

int main(int argc, char** argv)
{
    game = new GameParaPara(argc, argv);

    // init the game
    if(game->GetInitialised())
    {
        // Create sprite list
        sprites = new BL_SpriteList(game->GetMainRenderer(), "sprite_definitions.dat");
        SDL_SetTextureBlendMode(sprites->GetSprite(OBJ_SHOCKWAVE)->GetTexture(),
                                SDL_BLENDMODE_ADD);
        if(!sprites->GetInitialised())
            BL_EHLog("main(): Cannot load sprite definitions.\n");
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
BL_SpriteList* GetSpriteList()
{
    return sprites;
}