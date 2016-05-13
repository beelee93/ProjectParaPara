#ifndef _GAMEPARAPARA_H
#define _GAMEPARAPARA_H

#include "Game.h"
#include "ArrowList.h"

#define SND_BEEP 0
#define SND_SELECT 1
#define SND_CONFIRM 2

class GOFontSheet;
class GODefaultArrow;

enum GameState {
	GS_Null,                /* Used by fadeTarget */
	GS_Splash,              /* Splash screen */
	GS_MainMenu,            /* Press a button to begin */
	GS_SongSelection,       /* Menu to select songs */
	GS_Arena,               /* Main playing screen */
	GS_Scoreboard,          /* Results and score */
	GS_Calibration			/* To check if inputs are working */
};

/////// Inherited Game ////////
class GameParaPara : public Game
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
	void UpdateCalibration(double secs);

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
		int toggler;
		double timer;
	} MainMenu;

	struct {
		int currentSelection;
		GOFontSheet* songText;
		double timer;
	} SongSelection;

	struct {
		int index;
	} Scoreboard;

	struct {
		int started;
	} Calibration;

	// input polling
	InputParaPara* input;

	int targetScore;
	double score;

	friend void ArrowFailureHandler(GODefaultArrow*);
};
#endif