//////////////////////////////////////////
// Audio.h
// Contains the definitions for the audio 
// system in game
//////////////////////////////////////////
#ifndef _AUDIO_H_INCLUDED
#define _AUDIO_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include "ErrorHandler.h"

class BL_Audio
{
public:
	BL_Audio();
	~BL_Audio();

	int AddMusic(const char* filename);
	int AddSound(const char* filename);

	void PlayMusic(int index);
	void StopMusic();
	void PlaySound(int index);

	// remove from memory
	void PurgeMusic();
	void PurgeSound();

	int GetInitialised();

protected:
	Mix_Chunk** chunks;
	int chunkCapacity;
	int chunkCount;

	Mix_Music** musics;
	int currentMusic;
	int musicCapacity;
	int musicCount;
	int initialised;
};
#endif