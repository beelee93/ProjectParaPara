//////////////////////////////////////////
// InputParaPara.cpp
// Implements the input system
//////////////////////////////////////////

#include "InputParaPara.h"

InputParaPara::InputParaPara()
{
	this->currInput = this->prevInput = 0;
#ifdef RPI
	wiringPiSetup();
	for(int i=0;i<5;i++)
		pinMode(i, INPUT);
#endif
}

uint8_t InputParaPara::GetCurrentInput(int rpi)
{
	if (rpi != 0) rpi = 1;
#ifndef RPI
	rpi = 0;
#endif
	return (uint8_t)(this->currInput >> (rpi * 8));
}

uint8_t InputParaPara::GetPreviousInput(int rpi)
{
	if (rpi != 0) rpi = 1;
#ifndef RPI
	rpi = 0;
#endif
	return (uint8_t)(this->prevInput >> (rpi * 8));
}

uint8_t InputParaPara::GetRisingEdge(int rpi)
{
	if (rpi != 0) rpi = 1;
#ifndef RPI
	rpi = 0;
#endif
	uint16_t a = ~prevInput & currInput;
	return (uint8_t)(a >> (rpi * 8));
}
void InputParaPara::Reset()
{
	currInput = prevInput = 0;
}

void InputParaPara::Update()
{
	this->prevInput = this->currInput;

	this->currInput = 0;
#ifdef RPI
	int temp;
	for (int i = 0; i < 5; i++)
	{
		temp = digitalRead(i) ? 1 : 0;
		temp = temp << (8 + i);
		this->currInput |= temp;
	}
#else
	const uint8_t* keys = SDL_GetKeyboardState(NULL);
	this->currInput |= (
		keys[SDL_SCANCODE_Y] |
		keys[SDL_SCANCODE_U] << 1 |
		keys[SDL_SCANCODE_I] << 2 |
		keys[SDL_SCANCODE_O] << 3 |
		keys[SDL_SCANCODE_P] << 4);
#endif
}