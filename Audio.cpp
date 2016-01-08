//////////////////////////////////////////
// Audio.cpp
// Implements the Audio system
//////////////////////////////////////////

#include "Audio.h"

static char errMsg[256] = "";

// ctor
BL_Audio::BL_Audio()
{
	initialised = 0;
	musics = NULL;
	chunks = NULL;
	
	int b = Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
	if ((b & MIX_INIT_OGG) == 0 || (b & MIX_INIT_MP3) == 0)
	{
		BL_EHLog("BL_Audio::ctor(): Mix_Init failed.\n");
		return;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
	{
		BL_EHLog("BL_Audio::ctor(): Mix_OpenAudio failed.\n");
		return;
	}

	chunks = (Mix_Chunk**) malloc(sizeof(Mix_Chunk*) * 10);
	if (!chunks)
	{
		BL_EHLog("BL_Audio::ctor(): Cannot malloc chunk list.\n");
		return;
	}
	SDL_memset(chunks, 0, sizeof(Mix_Chunk*) * 10);
	chunkCapacity = 10;
	chunkCount = 0;

	musics = (Mix_Music**)malloc(sizeof(Mix_Music*) * 10);
	if (!musics)
	{
		BL_EHLog("BL_Audio::ctor(): Cannot malloc music list.\n");
		return;
	}
	SDL_memset(musics, 0, sizeof(Mix_Music*) * 10);
	currentMusic = -1;
	musicCapacity = 10;
	musicCount = 0;

	initialised = 1;
}

void BL_Audio::PurgeMusic()
{
	if (!musics) return;

	// free all music
	int i;
	for (i = 0; i < musicCapacity; i++)
	{
		if (musics[i] != NULL)
		{
			Mix_FreeMusic(musics[i]);
			musics[i] = NULL;
		}
	}
	musicCount = 0;
}

void BL_Audio::PurgeSound()
{
	if (!chunks) return;

	// free all chunks
	int i;
	for (i = 0; i < chunkCapacity; i++)
	{
		if (chunks[i] != NULL)
		{
			Mix_FreeChunk(chunks[i]);
			chunks[i] = NULL;
		}
	}
	chunkCount = 0;
}

int BL_Audio::AddMusic(const char* filename)
{
	if (!filename) return -1;
	if (!initialised) return -1;

	Mix_Music* mus = Mix_LoadMUS(filename);
	if (!mus)
	{
		sprintf(errMsg, "AddMusic(): Cannot load music file '%s'.\n", filename);
		BL_EHLog(errMsg);
		return -1;
	}

	if (musicCount >= musicCapacity)
	{
		// expand capacity
		Mix_Music** temp = (Mix_Music**)malloc(sizeof(Mix_Music*) * (musicCapacity+10));
		SDL_memset(musics, 0, sizeof(Mix_Music*) * (musicCapacity + 10));
		SDL_memcpy(temp, musics, sizeof(Mix_Music*) * musicCapacity);

		free(musics);
		musics = temp;
		musicCapacity += 10;
	}
	musics[musicCount++] = mus;

	return musicCount - 1;
}

int BL_Audio::AddSound(const char* filename)
{
	if (!filename) return -1;
	if (!initialised) return -1;

	Mix_Chunk* mus = Mix_LoadWAV(filename);
	if (!mus)
	{
		sprintf(errMsg, "AddSound(): Cannot load sound file '%s'.\n", filename);
		BL_EHLog(errMsg);
		return -1;
	}

	if (chunkCount >= chunkCapacity)
	{
		// expand capacity
		Mix_Chunk** temp = (Mix_Chunk**)malloc(sizeof(Mix_Chunk*) * (chunkCapacity + 10));
		SDL_memset(chunks, 0, sizeof(Mix_Chunk*) * (chunkCapacity + 10));
		SDL_memcpy(temp, chunks, sizeof(Mix_Chunk*) * chunkCapacity);

		free(chunks);
		chunks = temp;
		chunkCapacity += 10;
	}
	chunks[chunkCount++] = mus;

	return chunkCount - 1;
}

void BL_Audio::PlayMusic(int index)
{
	if (!initialised) return;
	if (index < 0 || index >= musicCapacity) return;
	if (!musics[index]) return;

	StopMusic();
	Mix_PlayMusic(musics[index], 0);
	currentMusic = index;
}

void  BL_Audio::StopMusic()
{
	if (currentMusic > -1)
	{
		Mix_HaltMusic();
		currentMusic = -1;
	}
}

int BL_Audio::GetInitialised()
{
	return initialised;
}

void BL_Audio::PlaySound(int index)
{
	if (!initialised) return;
	if (index < 0 || index >= chunkCapacity) return;
	if (!chunks[index]) return;
	Mix_PlayChannel(-1, chunks[index], 0);
}

BL_Audio::~BL_Audio()
{
	StopMusic();

	PurgeMusic();
	PurgeSound();

	if(musics)
		free(musics);
	
	if (chunks)
		free(chunks);

	Mix_CloseAudio();
	Mix_Quit();
}