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

static GOM* gom = NULL;
static FC_Font* statusFont = NULL;

static double timer = 0;

static GOStationaryArrow* bwArrows[5];

static int health = 100;
static int useRPI = 1;

static GameParaPara* theGame = NULL;

void ArrowFailureHandler(GODefaultArrow*);

///////////////////////////////////////////////////////////////////
GameParaPara::GameParaPara(int argc, char** argv) : Game(argc, argv)
{
	theGame = this;

    // set initial game state
    gameState = GS_Splash;
    fadeTimer = 0.0;
    fadeMode = 0;
    fadeTarget = GS_Null;
	
	SongSelection.currentSelection = 1;
	SongSelection.songText = NULL;
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
        EHLog("GameParaPara(): Failed to create font.\n");
        return;
    }

    // use a system installed font
    FC_LoadFont(statusFont, mainRenderer, SYSFONT,
         16, FC_MakeColor(255,255,255,255), TTF_STYLE_NORMAL);

	if (initialised)
	{
		if (!Audio::LoadMusicList("music.dat", audio))
		{
			this->initialised = 0;
			EHLog("GameParaPara(): Failed to load music list.\n");
			return;
		}
	}

	// load sounds
	/* 0 */ audio->AddSound("res/beep.wav");
	/* 1 */ audio->AddSound("res/music-select.wav");
	/* 2 */ audio->AddSound("res/music-confirm.wav");

	// create new input system
	input = new InputParaPara();

	// create arrow list
	Arena.arrowList = new ArrowList();

#ifdef RPI
	useRPI = 1;
#endif
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
    Game::OnUpdate(secs);

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

	case GS_Calibration:
		UpdateCalibration(secs);
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
    Game::OnEvent(event, secs);

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
					{
						audio->StopMusic();
						SongSelection.songText = NULL;
						FadeToGameState(GS_MainMenu);
					}
					else if (gameState == GS_Scoreboard)
						FadeToGameState(GS_MainMenu);
                    break;
				case SDLK_F10: // toggle rpi gpio pins usage
#ifdef RPI
					useRPI = useRPI? 0:1;
					printf("UseRPI=%d\n", useRPI);
#endif
					break;

				case SDLK_F9:
					if (gameState == GS_MainMenu)
						ChangeGameState(GS_Calibration);
					else if (gameState == GS_Calibration)
						ChangeGameState(GS_MainMenu);
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
	char msg1[20]="Select Song";
	GameObject* temp1;
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

	case GS_SongSelection:
		SongSelection.currentSelection = 0;
		temp1 = objManager->CreateObject(OBJ_FONTSHEET, msg1);
		((GOFontSheet*)temp1)->SetCentered(1);
		temp1->x = 400;
		temp1->y = 10;
		SongSelection.songText = (GOFontSheet*)objManager->CreateObject(OBJ_FONTSHEET, audio->GetMusicInformation(0)->name);
		SongSelection.songText->x = 400;
		SongSelection.songText->y = 280;
		SongSelection.songText->SetCentered(1);
		SongSelection.timer = 0;
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

	case GS_Calibration:
		Calibration.started = 0;
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
//#define EDITOR
#ifdef EDITOR
static int started = 0;
static int timeStamps[256] = { 0 };
static int timeStampIndex = 0;
#endif


void GameParaPara::UpdateMainMenu(double secs)
{
	MainMenu.timer += secs;
	if (MainMenu.timer > 1)
	{
		MainMenu.timer = 0;
		MainMenu.toggler ^= 1;
	}

#ifndef EDITOR
	if (fadeMode) return;
	uint8_t inp = input->GetRisingEdge(useRPI);
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
	
#else
	uint8_t inp = input->GetRisingEdge(useRPI);
	if (!started)
	{
		timer = 0;
		if (inp & 1) {
			started = 1;
			audio->PlayMusic(1);
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
#endif
}

void GameParaPara::UpdateSongSelection(double secs)
{
	if (SongSelection.timer < 1 && fadeMode != FM_FADE_OUT)
	{
		SongSelection.timer += secs;
		if (SongSelection.timer >= 1)
			audio->PlayMusic(0);
	}

	if (fadeMode) return;

	int changed = 0;
	uint8_t inp = input->GetRisingEdge(useRPI);
	if (inp & 1) // To left
	{
		audio->PlaySound(SND_SELECT);
		audio->StopMusic();
		SongSelection.currentSelection--;

		if (SongSelection.currentSelection < 0)
			SongSelection.currentSelection = audio->GetMusicCount() - 1;

		changed = 1;

	}
	else if (inp & 16) // To right
	{
		audio->PlaySound(SND_SELECT);
		audio->StopMusic();
		SongSelection.currentSelection++;

		if (SongSelection.currentSelection >= audio->GetMusicCount())
			SongSelection.currentSelection = 0;

		changed = 1;
	}

	if (changed)
	{
		// play the newly selected music
		audio->PlayMusic(SongSelection.currentSelection);

		// set the text
		if (SongSelection.songText)
			SongSelection.songText->SetString(audio->GetCurrentMusic()->name);
	}
	

	if (inp & 4) // center
	{
		audio->PlaySound(SND_CONFIRM);
		audio->PauseMusic();
		FadeToGameState(GS_Arena);
	}
}

void GameParaPara::UpdateScoreboard(double secs)
{
	if (fadeMode) return;

	uint8_t inp = input->GetRisingEdge(useRPI);
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

void GameParaPara::UpdateCalibration(double secs)
{
	int tempArenaX;
	GameObject* tempObj;
	uint8_t currInput;

	if (!Calibration.started)
	{
		for (tempArenaX = 0; tempArenaX < 5; tempArenaX++)
		{
			bwArrows[tempArenaX] = (GOStationaryArrow*)objManager->CreateObject(
				OBJ_DEFAULT_ARROWS_BW, &tempArenaX);
			tempObj = objManager->CreateObject(OBJ_PINK_FLASH, &tempArenaX);
			bwArrows[tempArenaX]->SetAttachedFlash((GOPinkFlash*)tempObj);
		}
		Calibration.started = 1;
	}
	else
	{
		currInput = input->GetCurrentInput(useRPI);
		for (tempArenaX = 0; tempArenaX < 5; tempArenaX++) // loop through each column
		{
			if (currInput & (1 << tempArenaX))
			{
				// we flash the corresponding arrow
				bwArrows[tempArenaX]->Flash();
			}
		}
	}
}

void GameParaPara::UpdateArena(double secs)
{
	int tempArenaX, tempArenaY;
	double timeDiff;
	GameObject* tempObj;
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
			audio->PlayMusic(SongSelection.currentSelection);
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
		riseKey = input->GetRisingEdge(useRPI);
		currInput = input->GetCurrentInput(useRPI);

		// look for flying arrows
		tempRect.x = 208;
		tempRect.y = 90;
		tempRect.w = 384;
		tempRect.h = 2;
		GameObject** nearObjs = (GameObject**)objManager->FindObjectsOfType(
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

//		theGame->isVictory = 0;
//		theGame->FadeToGameState(GS_Scoreboard);
	}
}
