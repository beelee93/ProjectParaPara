//////////////////////////////////////////
// Audio.h
// Defines a class to handle audio
//////////////////////////////////////////
#ifndef BL_AUDIO_H_INCLUDED
#define BL_AUDIO_H_INCLUDED

#include "irrKlang.h"
#include "ErrorHandler.h"
#include <stdlib.h>
#include <string.h>

using namespace irrklang;

typedef struct
{
    ISoundSource* src;
    int isMusic;
} AudioProperties;

class BL_Audio
{
public:
    BL_Audio();
    ~BL_Audio();

    // add a new audio source from file
    // returns its unique ID (error returns -1)
    // music files are handled differently
    int AddAudio(const char* filename, int isMusic);

    // Plays the sound indicated by its id returned by
    // AddAudio. Returns NULL normally, unless the sound
    // reuqested is a music, which returns the pointer to
    // the music's ISound object.
    ISound* PlayAudio(int soundId, int musicStartPaused=1);

    int GetCapacity();
    int GetCount();
    int GetInitialised();

private:
    ISoundEngine* soundEngine;
    int capacity;
    int count;
    int initialised;
    AudioProperties** sources;
};

#endif // BL_AUDIO_H_INCLUDED
