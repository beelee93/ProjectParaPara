//////////////////////////////////////////
// Audio.cpp
// Implements the audio handling class
//////////////////////////////////////////

#include "Audio.h"
using namespace irrklang;

BL_Audio::BL_Audio()
{
    capacity = 20;
    count = 0;
    soundEngine = NULL;

    sources = (AudioProperties**)malloc(sizeof(AudioProperties*)*20);
    if(!sources)
    {
        BL_EHLog("BL_Audio(): Could not allocate memory for sound source collection.\n");
        goto err;
    }

    memset(sources, 0, sizeof(AudioProperties*)*20);

    soundEngine = createIrrKlangDevice();
    if(!soundEngine)
    {
        BL_EHLog("BL_Audio(): Could not create sound engine.\n");
        goto err;
    }

    initialised = 1;
    return;

    err:
        initialised = 0;
}

BL_Audio::~BL_Audio()
{
    int i=0;

    if(soundEngine)
    {
        // free any playing music


        // stop all other sounds
        soundEngine->stopAllSounds();

        // free the sound engine
        soundEngine->drop();
        soundEngine = NULL;
    }

    if(sources)
    {
        // free collection of AudioProperties
        for(i=0;i<capacity;i++)
        {
            if(sources[i])
            {
                free(sources[i]);
                sources[i]=NULL;
            }
        }
        free(sources);
    }
}

int BL_Audio::GetCapacity()
{
    return capacity;
}

int BL_Audio::GetCount()
{
    return count;
}

int BL_Audio::GetInitialised()
{
    return initialised;
}

int BL_Audio::AddAudio(const char* filename, int isMusic)
{
    if(soundEngine && initialised)
    {
        int i = 0;
        AudioProperties* prop = (AudioProperties*)malloc(sizeof(AudioProperties));

        if(!prop)
        {
            BL_EHLog("AddAudio(): Failed to allocate memory for AudioProperties.\n");
            return -1;
        }

        prop->isMusic=isMusic;
        prop->src = soundEngine->addSoundSourceFromFile(filename,ESM_AUTO_DETECT,true);

        if(!prop->src)
        {
            free(prop);
            BL_EHLog("AddAudio(): Failed to add sound source from file.\n");
            return -1;
        }

        // check capacity
        if(count>=capacity)
        {
            // expand capacity
            int newCap = capacity + 10;
            AudioProperties** tempArr = (AudioProperties**)malloc(sizeof(AudioProperties*)*newCap);

            if(!tempArr)
            {
                BL_EHLog("AddAudio(): Failed to expand capacity.\n");
                soundEngine->removeSoundSource(prop->src);
                free(prop);
                return -1;
            }

            memset(tempArr, 0, sizeof(AudioProperties*)*newCap); // zero out everything
            memcpy(tempArr, sources, sizeof(AudioProperties*)*capacity); // copy old array in

            free(sources); // free the old array
            sources = tempArr;
            i=count;
            capacity = newCap;
        }

        // look for the first free slot
        for(;i<capacity;i++)
        {
            if(sources[i]==NULL)
            {
                sources[i]=prop;
                return i;
            }
        }

        BL_EHLog("AddAudio(): No empty slot? \n");
        return -1; // for some stupid reason we reach here, we return 0
    }
    else
    {
        BL_EHLog("AddAudio(): Sound engine not initialised properly.\n");
        return -1;
    }
}


ISound* BL_Audio::PlayAudio(int id, int musicStartPaused)
{
    if(!soundEngine || !initialised) return NULL;

    AudioProperties* aud = NULL;
    if(id<0 || id>=capacity) return NULL;
    aud = sources[id];
    if(!aud || !aud->src) return NULL;

    // now we play the sound
    if(aud->isMusic)
    {
        return soundEngine->play2D(aud->src, false, musicStartPaused ? true : false, true, false);
    }
    else
    {
        soundEngine->play2D(aud->src);
        return NULL;
    }
}
