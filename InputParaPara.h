//////////////////////////////////////////
// InputParaPara.h
// Defines the class that polls input
// from keyboard/GPIO pins
//////////////////////////////////////////

// define RPI to compile include wiringPi codes
#ifndef _INPUTPARAPARA_H
#define _INPUTPARAPARA_H

#ifdef RPI
#include <wiringPi.h>
#endif

#include <SDL.h>

class InputParaPara {
public:
	InputParaPara();

	uint8_t GetCurrentInput(int rpi=0);
	uint8_t GetPreviousInput(int rpi=0);
	uint8_t GetRisingEdge(int rpi=0);

	void Update();
	void Reset();
private:
	uint16_t currInput;
	uint16_t prevInput;

};

#endif