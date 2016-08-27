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
	wiringPiSPISetup(0, SPI_SPEED);
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

static unsigned char spidata[3] = { 0, 0, 0 };

static unsigned int sensorValue[5][5] = { 0 };

static void putSensorValue(int i, int val) 
{
	int k;
	for(k=4;k>0;k--) 
	{
		sensorValue[i][k] = sensorValue[i][k-1];
	}
	sensorValue[i][0]=val;
}

void InputParaPara::Update()
{
	this->prevInput = this->currInput;

	this->currInput = 0;
#ifdef RPI
	int temp;
	int sensor;
	
	// read CH0 to CH4
	for (int i = 0; i < 5; i++)
	{
		// set the data to be sent to the sensor
		spidata[0] = 0x1;
		spidata[1] = (8 + i) << 4;
		spidata[2] = 0;

		// simultaneous write/read
		wiringPiSPIDataRW(0, spidata, 3);

		// parse data (take 10 bits)
		sensor = ((spidata[1] & 0x7)<<8) + spidata[2];

		putSensorValue(i, sensor);

		// the larger the sensor value, the closer the obstacle
		sensor = 0;
		for(int j=0;j<5;j++) 
		{
			sensor+=sensorValue[i][j];
		}	
		sensor/=5;

		temp = (sensor > SENSOR_THRESHOLD ? 1 : 0);
		temp = temp << (8 + i);
		this->currInput |= temp;
	}
#endif

	// get keyboard state
	const uint8_t* keys = SDL_GetKeyboardState(NULL);
	this->currInput |= (
		keys[SDL_SCANCODE_Y] |
		keys[SDL_SCANCODE_U] << 1 |
		keys[SDL_SCANCODE_I] << 2 |
		keys[SDL_SCANCODE_O] << 3 |
		keys[SDL_SCANCODE_P] << 4);
}
