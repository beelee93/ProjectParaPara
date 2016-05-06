#ifndef _GAMEPARAPARA_H
#define _GAMEPARAPARA_H

#include "Game.h"
#include "ArrowList.h"

#define SND_BEEP 0

class GODefaultArrow;

enum GameState {
	GS_Null,                /* Used by fadeTarget */
	GS_Splash,              /* Splash screen */
	GS_MainMenu,            /* Press a button to begin */
	GS_SongSelection,       /* Menu to select songs */
	GS_Arena,               /* Main playing screen */
	GS_Scoreboard          /* Results and score */
};

/////// Inherited Game ////////
class GameParaPara : public BL_Game
{
public:
	GameParaPara(int, char**);
	~GameParaPara();
	void OnUpdate(double secs);
	void OnEvent(SDL_Event* event, double secs);
	void OnRender(SDL_Renderer* renderer, double secs);
	void OnExitState(GameState leavingState);
	void OnEnterState(GameState newState);
	void ChangeGameState(GameState newState);
	void FadeToGameState(GameState state);

	void PollInput();

	// Update functions
	void UpdateArena(double secs);
	void UpdateSongSelection(double secs);
	void UpdateMainMenu(double secs);
	void UpdateScoreboard(double secs);

protected:
	GameState gameState;
	int isVictory;

	double timer;

	// Transition fade mechanism
	double fadeTimer;
	int fadeMode;
	GameState fadeTarget;

	// Arena variables
	struct {
		int arenaStarted;
		ArrowList* arrowList;
	} Arena;

	struct {
		int index;
	} MainMenu;

	struct {
		int currentSelection;
	} SongSelection;

	struct {
		int index;
	} Scoreboard;

	// input polling
	InputParaPara* input;

	friend void ArrowFailureHandler(GODefaultArrow*);
};
#endif