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

typedef struct {
	char name[50];
	char arrowDef[128];
	double length;
	Mix_Music* music;
} MusicInformation;

class BL_Audio
{
public:
	BL_Audio();
	~BL_Audio();

	int AddMusic(const char* filename, const char* name, double length, const char* arrowDef);
	int AddSound(const char* filename);

	void PlayMusic(int index);
	void StopMusic();
	void PlaySound(int index);

	// remove from memory
	void PurgeMusic();
	void PurgeSound();

	int GetInitialised();
	int GetMusicCount();

	MusicInformation* GetCurrentMusic();
	MusicInformation* GetMusicInformation(int index);
	static int LoadMusicList(const char* filename, BL_Audio* audio);

protected:
	Mix_Chunk** chunks;
	int chunkCapacity;
	int chunkCount;

	MusicInformation* musicInfo;
	int currentMusic;
	int musicCapacity;
	int musicCount;
	int initialised;
};
#endif