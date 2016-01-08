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
#define OBJ_PINK_FLASH          9000


#include "GameObjectManager.h"
#include "Game.h"
#include "SpriteLoader.h"
#include "SDL_FontCache.h"

#include "GameParaPara.h"

#if !defined(WIN32)
#include <bcm2835.h>
#define PIN_A	RPI_V2_GPIO_P1_07
#define PIN_B	RPI_V2_GPIO_P1_11
#define PIN_C	RPI_V2_GPIO_P1_13
#define PIN_D	RPI_V2_GPIO_P1_15
#define PIN_E	RPI_V2_GPIO_P1_16
#endif 

//#include "bcm2835.h"


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
#define ARROW_SPEED     200
#define FlightTime      ((600.0 - ARROW_TARGET_Y) / (ARROW_SPEED))

class GODefaultArrow : public BL_GameObject
{
public:
    GODefaultArrow();
    void OnInit(int id, int type, void* data=NULL);
    void OnUpdate(double secs);
    void Disappear();
	int HasInput();
	void SetInput();
private:
    int disappearing;
	int xs, ys;
	int hasInput;
};

class GOPinkFlash : public BL_GameObject
{
public:
    GOPinkFlash();
    void OnInit(int id, int type, void* data=NULL);
    void OnUpdate(double secs);
    void Flash();
    void OnRender(double secs);
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
#endif // RESOURCE_H_INCLUDED
