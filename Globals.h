//////////////////////////////////////////
// Globals.h
// Defines constants for resource ids
// Sprite ids should coincide with their
// object type ids
// Also defines inheritances
//////////////////////////////////////////
#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED

/////// Config ////////
#define USE_KEYBOARD_INPUT

/////// Objects ////////
#define OBJ_DEFAULT_ARROWS      0
#define OBJ_DEFAULT_ARROWS_BW   1
#define OBJ_SHOCKWAVE           2
#define SPR_CHAINED_BACK		3 /* Not used as object */
#define SPR_NUMBERS				4
#define OBJ_HEADER				5
#define OBJ_TITLE				6
#define SPR_SORT_OF				7
#define SPR_HEALTHBAR			8

#define OBJ_SCORE				9001
#define OBJ_PINK_FLASH          9000


#include "GameObjectManager.h"
#include "Game.h"
#include "SpriteLoader.h"
#include "SDL_FontCache.h"

#include "InputParaPara.h"
#include "GameParaPara.h"

/////// Constants ////////

#define FM_FADE_OUT -1
#define FM_FADE_IN 1
#define FM_FADE_STOP 0


/////// Global Functions ////////
BL_SpriteList* GetSpriteList();

/////// Inherited GOM ////////
class GOMParaPara : public BL_GOM
{
public:
    GOMParaPara();
    BL_GameObject* OnCreateObject(int type);
};

/////// Objects Classes ////////
#define ARROW_TARGET_Y  64
#define ARROW_SPEED     300
#define FlightTime      ((600.0 - ARROW_TARGET_Y) / (ARROW_SPEED))

#define HIBITS(x) ( x>>4 )
#define LOBITS(x) ( x & (0x0F) )

typedef struct {
	char flags;		// HI-bits - arrow index; LO-bits - chained?
	float chainDelay;
} GODefaultArrowData;

class GODefaultArrow : public BL_GameObject
{
public:
    GODefaultArrow();
    void OnInit(int id, int type, void* data=NULL);
    void OnUpdate(double secs);
	void OnRender(double secs, SDL_Renderer *renderer);
    void Disappear();
	int HasInput();
	void SetInput();
	int IsChained();
	int GetChainSuccess();
	int IsChainBroken();
	int IsDisappearing();
	void FlagForChain(int flag);
	void SetAccuracy(double accu);
	double GetAccuracy();
	void SetFailedCallback(void (*callback)(GODefaultArrow*));

private:
    int disappearing; // is the arrow disappearing?
	int chainInput; // This has to be 1. If this is 0, the chain is broken prematurely.
	int chainSuccess; // This is set to 1 when the chain is complete
	int chainBroken;
	int chainBrokenAcknowledged;
	void (*failedCallback)(GODefaultArrow*);

	int colX;
	double accuracy;
	GODefaultArrowData attachedData;
	int xs, ys;		  // (x,y) upon invoking Disappear
	int hasInput;	  // has already validated a user input

};

class GOPinkFlash : public BL_GameObject
{
public:
    GOPinkFlash();
    void OnInit(int id, int type, void* data=NULL);
    void OnUpdate(double secs);
    void Flash();
    void OnRender(double secs, SDL_Renderer* renderer);
private:
    SDL_Rect drawRect;
};

class GOStationaryArrow : public BL_GameObject
{
public:
    GOStationaryArrow();
    void OnInit(int id, int type, void* data=NULL);
    void OnUpdate(double secs);
    void Flash();

    void SetAttachedFlash(GOPinkFlash* flash);

private:
    GOPinkFlash* flasher;
};

class GOShockwave : public BL_GameObject
{
public:
    GOShockwave();
    void OnInit(int id, int type, void* data = NULL);
    void OnUpdate(double secs);

private:
    int firstUpdate;
    double xstart, ystart;
	float scaler;
};

class GOTitle : public BL_GameObject
{
public:
	GOTitle();
	void OnInit(int id, int type, void* data = NULL);
	void OnUpdate(double secs);
	void OnRender(double secs, SDL_Renderer* renderer);

private:
	double timer;
	double backAlpha;
	BL_Sprite* sortOfSprite;
};

class GOScore : public BL_GameObject
{
public:
	GOScore();
	void OnInit(int id, int type, void* data = NULL);
	void OnRender(double secs, SDL_Renderer* renderer);

private:
	double* score;

};

class GOHeader : public BL_GameObject
{
public:
	GOHeader();
	void OnInit(int id, int type, void* data = NULL);
	void OnRender(double secs, SDL_Renderer* renderer);

private: 
	int* health;
	BL_Sprite* barSprite;
	SDL_Rect drawRect;
};
#endif // RESOURCE_H_INCLUDED
