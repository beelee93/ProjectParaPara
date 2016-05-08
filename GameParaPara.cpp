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

static GOStationaryArrow* bwArrows[5];

static int health = 100;

static GameParaPara* theGame = NULL;

///////////////////////////////////////////////////////////////////
GameParaPara::GameParaPara(int argc, char** argv) : BL_Game(argc, argv)
{
	theGame = this;

    // set initial game state
    gameState = GS_Splash;
    fadeTimer = 0.0;
    fadeMode = 0;
    fadeTarget = GS_Null;
	
	SongSelection.currentSelection = 0;
	MainMenu.toggler = 0;

    // initialise gom
    gom = new GOMParaPara();
    this->SetObjectManager(gom);

    // attach renderer to gom
    gom->SetAttachedRenderer(GetMainRenderer());

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

	if (initialised)
	{
		if (!BL_Audio::LoadMusicList("music.dat", audio))
		{
			this->initialised = 0;
			BL_EHLog("GameParaPara(): Failed to load music list.\n");
			return;
		}
	}

	// load sounds
	/* 0 */ audio->AddSound("res/beep.wav");

	// create new input system
	input = new InputParaPara();

	// create arrow list
	Arena.arrowList = new ArrowList();
}

///////////////////////////////////////////////////////////////////
GameParaPara::~GameParaPara()
{
	// free input
	delete input;

    // free GOM
    if(gom) delete gom;

    // free fonts
    if(statusFont) FC_FreeFont(statusFont);

	// free arrow list
	if(Arena.arrowList)
		delete Arena.arrowList;
	Arena.arrowList = NULL;
}

///////////////////////////////////////////////////////////////////
void GameParaPara::OnUpdate(double secs)
{
    BL_Game::OnUpdate(secs);

	// Poll inputs
	input->Update();

	SDL_Rect tempRect = {0};

	switch (gameState)
	{
		//////////// UPDATE FOR SPLASHSCREEN ////////////
	case GS_Splash:
		timer -= secs;
		if (timer <= 0)
		{
			ChangeGameState(GS_MainMenu);
			timer = 0;
		}
		break;

		//////////// UPDATE FOR ARENA ////////////
	case GS_Arena:
		UpdateArena(secs);
		break;

		/////////// UPDATE FOR MAIN MENU //////////
	case GS_MainMenu:
		UpdateMainMenu(secs);
		break;

		/////////// UPDATE FOR SONG SELECTION //////////
	case GS_SongSelection :
		UpdateSongSelection(secs);
		break;

	case GS_Scoreboard: 
		UpdateScoreboard(secs);
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
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	if (objManager) objManager->Render(secs);
    switch(gameState)
    {
    case GS_Arena:
        break;

	case GS_MainMenu:
		if(MainMenu.toggler)
			FC_Draw(statusFont, renderer, 312, 400, "Swipe left to right to begin...");
		break;
    }


    // Transition fade out mechanism
    if(fadeMode)
    {
        SDL_SetRenderDrawColor(renderer,0,0,0,(int)(fadeTimer * 255));
        SDL_RenderFillRect(renderer, NULL);
    }

	/*
    FC_Draw(statusFont, renderer, 3,3, "FPS: %d\nGameState: %d",
            (int)(1/secs), gameState);
			*/
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
					else if (gameState == GS_Arena)
						FadeToGameState(GS_SongSelection);
					else if (gameState == GS_SongSelection)
						FadeToGameState(GS_MainMenu);
					else if (gameState == GS_Scoreboard)
						FadeToGameState(GS_SongSelection);
                    break;
                case SDLK_a:
                    GetObjectManager()->CreateObject(OBJ_DEFAULT_ARROWS);
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
		audio->StopMusic();
        break;

	case GS_SongSelection:
		// load arrow list
		Arena.arrowList->Clear();
		Arena.arrowList->LoadDefinitionFile(audio->GetMusicInformation(SongSelection.currentSelection)->arrowDef);
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

	case GS_MainMenu:
		timer = 0;
		MainMenu.index = 0;
		MainMenu.toggler = 0;
		MainMenu.timer = 0;
		input->Reset();

		// create title
		gom->CreateObject(OBJ_TITLE);

		break;
    case GS_Arena:
		health = 100;
        timer = 2.0;
        Arena.arenaStarted = 0;
		input->Reset();
		score = 0;
		targetScore = 0;

		// create header
		gom->CreateObject(OBJ_HEADER, &health);

		// create score
		gom->CreateObject(OBJ_SCORE, &score);
        break;

	case GS_Scoreboard:
		Scoreboard.index = 0;
		timer = 0;
		input->Reset();
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

///////////////////////////////////////////////////////////////////
// UPDATE FUNCTIONS
///////////////////////////////////////////////////////////////////
static int started = 0;
static int timeStamps[256] = { 0 };
static int timeStampIndex = 0;
void GameParaPara::UpdateMainMenu(double secs)
{
	MainMenu.timer += secs;
	if (MainMenu.timer > 1)
	{
		MainMenu.timer = 0;
		MainMenu.toggler ^= 1;
	}

	if (fadeMode) return;
	uint8_t inp = input->GetRisingEdge();
	if (inp & (1 << MainMenu.index))
	{
		MainMenu.index++;
		audio->PlaySound(SND_BEEP);

		timer = 0.5;
		if (MainMenu.index >= 5) {
			FadeToGameState(GS_SongSelection);
		}
	}

	timer -= secs;
	if (timer <= 0)
	{
		MainMenu.index = 0;
		timer = 0;
	}
	
	/*
	uint8_t inp = input->GetRisingEdge();
	if (!started)
	{
		timer = 0;
		if (inp & 1) {
			started = 1;
			audio->PlayMusic(0);
		}
	}
	else
	{
		timer += secs;
		if (inp & 1) {
			timeStamps[timeStampIndex++] = (int)(timer * 1000);
		}

		if (inp & 1<<4) {
			started = 0;
			FILE* file = fopen("arrow.txt", "w+");
			for(int i=0;i<timeStampIndex;i++)
				fprintf(file, "%d,%d\n", i+1, timeStamps[i]);
			fclose(file);
		}
	}
	*/
}

void GameParaPara::UpdateSongSelection(double secs)
{
	const uint8_t* k = SDL_GetKeyboardState(NULL);
	if (k[SDL_SCANCODE_RETURN])
	{
		FadeToGameState(GS_Arena);
	}
}

void GameParaPara::UpdateScoreboard(double secs)
{
	if (fadeMode) return;

	uint8_t inp = input->GetRisingEdge();
	if (inp & (1 << Scoreboard.index))
	{
		Scoreboard.index++;
		audio->PlaySound(SND_BEEP);

		timer = 0.5;
		if (Scoreboard.index >= 5) {
			FadeToGameState(GS_SongSelection);
		}
	}
}

void GameParaPara::UpdateArena(double secs)
{
	int tempArenaX, tempArenaY;
	double timeDiff;
	BL_GameObject* tempObj;
	GODefaultArrow* tempArrow;
	GODefaultArrowData tempArrowData = { 0 };
	ArrowListNode* curNode;

	SDL_Rect tempRect;
	uint8_t riseKey, currInput;

	if (!Arena.arenaStarted)
	{
		// Preparations
		timer -= secs;
		if (timer <= 0)
		{
			Arena.arenaStarted = 1;

			// play arena song
			audio->PlayMusic(0);
			timer = 0;

			// reset arrow list node pointer
			Arena.arrowList->ResetCurrentNode();

			// create the stationary arrows
			for (tempArenaX = 0; tempArenaX < 5; tempArenaX++)
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
		timer += secs;

		if (!fadeMode && timer>(audio->GetCurrentMusic()->length)) {
			// Victory
			isVictory = 1;
			FadeToGameState(GS_Scoreboard);
		}

		while (Arena.arrowList->GetCurrentNode() /* make sure current node is not NULL */ &&
			(timeDiff = (timer - (Arena.arrowList->GetCurrentNode()->timeStamp - FlightTime))) >= 0 /* timing */)
		{
			curNode = Arena.arrowList->GetCurrentNode();
			for (tempArenaX = 0; tempArenaX < 5; tempArenaX++)
			{
				if (ArrowIsEnabled(&(curNode->arrows[tempArenaX])))
				{
					tempArrowData.flags = (tempArenaX << 4);
					if (ArrowIsChained(&(curNode->arrows[tempArenaX])))
					{
						tempArrowData.flags |= 1;
						tempArrowData.chainDelay = curNode->arrows[tempArenaX].chainDelay;
					}
					tempObj = objManager->CreateObject(OBJ_DEFAULT_ARROWS, &tempArrowData);

					// Adjust starting y position based on timer - timeStamp difference
					tempObj->y -= ARROW_SPEED * timeDiff;

					// set the failure callback
					((GODefaultArrow*)tempObj)->SetFailedCallback(ArrowFailureHandler);
				}
			}
			Arena.arrowList->NextNode();
		}

		// Poll input
		riseKey = input->GetRisingEdge();
		currInput = input->GetCurrentInput();

		// look for flying arrows
		tempRect.x = 208;
		tempRect.y = 90;
		tempRect.w = 384;
		tempRect.h = 2;
		BL_GameObject** nearObjs = (BL_GameObject**)objManager->FindObjectsOfType(
			OBJ_DEFAULT_ARROWS, &tempRect);

		for (tempArenaX = 0; tempArenaX < 5; tempArenaX++) // loop through each column
		{
			if (currInput & (1 << tempArenaX))
			{
				// we flash the corresponding arrow
				bwArrows[tempArenaX]->Flash();
			}
		}

		// loop thru every detected object
		for (tempArenaY = 0; (tempArrow=(GODefaultArrow*)nearObjs[tempArenaY]) != NULL; tempArenaY++)
		{
			tempArenaX = (int)(tempArrow->imageIndex);
			if (tempArrow->IsChained())
			{
				// this is a chained arrow

				// check if arrow has been activated
				if (tempArrow->HasInput())
				{
					// provide input to arrow
					if (currInput & (1 << tempArenaX))
					{
						tempArrow->FlagForChain(1);
						targetScore += (int)(500 * tempArrow->GetAccuracy() * secs);
					}
					else
						tempArrow->FlagForChain(0);
				}
				else if ((riseKey & (1 << tempArenaX)) != 0) // arrow has not been activated
				{
					// look for rising edge
					tempArrow->SetAccuracy(1 - ((tempArrow->y > 64 ? tempArrow->y : 64) - 64) / 64.0);
					tempArrow->y = bwArrows[tempArenaX]->y;
					tempArrow->SetInput();
					tempArrow->FlagForChain(1);
					
				}

				// check for success
				if (tempArrow->GetChainSuccess())
				{
					// arrow chain success
					targetScore += (int)(tempArrow->GetAccuracy()*2000 * secs);

					// make arrow disappear
					if(!tempArrow->IsDisappearing())
						tempArrow->Disappear();

					// create a shockwave at arrow
					tempObj = objManager->CreateObject(OBJ_SHOCKWAVE);
					tempObj->x = bwArrows[tempArenaX]->x + 32;
					tempObj->y = bwArrows[tempArenaX]->y + 32;

					// increase health
					health += 5;
					if (health > 100) health = 100;
				}
			}
			else
			{
				// this is a singular arrow

				// check if this arrow has already been activated
				if (tempArrow->HasInput())
					continue; // next arrow plz

				// check if riseKey corresponds to imageIndex
				if ((riseKey & (1 << (int)(tempArrow->imageIndex))) == 0)
					continue;

				// has not fully disappeared
				if (tempArrow->alpha > 0.2)
				{
					targetScore += (int)((1 - ((tempArrow->y > 64 ? tempArrow->y : 64) - 64) / 64.0) * 1000 * secs);

					// make it disappear
					tempArrow->y = bwArrows[tempArenaX]->y;
					tempArrow->SetInput();

					// create a shockwave at arrow
					tempObj = objManager->CreateObject(OBJ_SHOCKWAVE);
					tempObj->x = bwArrows[tempArenaX]->x + 32;
					tempObj->y = bwArrows[tempArenaX]->y + 32;

					// increase health
					health += 2;
					if (health > 100) health = 100;
				}
			}

		}

		// update score
		double diff;
		if (score < targetScore)
		{
			diff = targetScore - score;
			if (diff < 50) diff = 50;
			score += diff * secs;
			if (score > targetScore) score = targetScore;
		}
	}
}

// Handles arrow failures
void ArrowFailureHandler(GODefaultArrow* target)
{
	health -= 5;

	if (health < 0) {
		health = 0;

		theGame->isVictory = 0;
		theGame->FadeToGameState(GS_Scoreboard);
	}
}