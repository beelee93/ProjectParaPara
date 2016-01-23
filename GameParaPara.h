#include "Game.h"
#include "ArrowList.h"

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

protected:
	GameState gameState;
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

	// Song names
	char** songNames;
	int songCount;

	// input polling
	uint8_t currInput;
	uint8_t prevInput;
};
