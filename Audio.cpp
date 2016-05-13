//////////////////////////////////////////
// Audio.cpp
// Implements the Audio system
//////////////////////////////////////////

#include "Audio.h"

static char errMsg[256] = "";

// ctor
Audio::Audio()
{
	initialised = 0;
	musicInfo = NULL;
	chunks = NULL;
	
	int b = Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
	if ((b & MIX_INIT_OGG) == 0 || (b & MIX_INIT_MP3) == 0)
	{
		EHLog("Audio::ctor(): Mix_Init failed.\n");
		return;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
	{
		EHLog("Audio::ctor(): Mix_OpenAudio failed.\n");
		return;
	}

	chunks = (Mix_Chunk**) malloc(sizeof(Mix_Chunk*) * 10);
	if (!chunks)
	{
		EHLog("Audio::ctor(): Cannot malloc chunk list.\n");
		return;
	}
	SDL_memset(chunks, 0, sizeof(Mix_Chunk*) * 10);
	chunkCapacity = 10;
	chunkCount = 0;

	musicInfo = (MusicInformation*)malloc(sizeof(MusicInformation) * 10);
	if (!musicInfo)
	{
		EHLog("Audio::ctor(): Cannot malloc music list.\n");
		return;
	}
	SDL_memset(musicInfo, 0, sizeof(MusicInformation) * 10);

	currentMusic = -1;
	musicCapacity = 10;
	musicCount = 0;

	initialised = 1;
}

void Audio::PurgeMusic()
{
	if (!musicInfo) return;

	// free all music
	int i;
	for (i = 0; i < musicCapacity; i++)
	{
		if (musicInfo[i].music != NULL)
		{
			Mix_FreeMusic(musicInfo[i].music);
			musicInfo[i].music = NULL;
		}
	}
	musicCount = 0;
}

void Audio::PurgeSound()
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

int Audio::AddMusic(const char* filename, const char* name, double length, const char* arrowDef)
{
	if (!filename) return -1;
	if (!initialised) return -1;

	Mix_Music* mus = Mix_LoadMUS(filename);
	if (!mus)
	{
		sprintf(errMsg, "AddMusic(): Cannot load music file '%s'.\n", filename);
		EHLog(errMsg);
		return -1;
	}

	if (musicCount >= musicCapacity)
	{
		// expand capacity
		MusicInformation* temp = (MusicInformation*)malloc(sizeof(MusicInformation) * (musicCapacity + 10));
		SDL_memset(temp, 0, sizeof(MusicInformation) * (musicCapacity + 10));
		SDL_memcpy(temp, musicInfo, sizeof(MusicInformation) * musicCapacity);

		free(musicInfo);
		musicInfo = temp;
		musicCapacity += 10;
	}
	
	musicInfo[musicCount].length = length;
	musicInfo[musicCount].music = mus;
	SDL_memcpy(musicInfo[musicCount].arrowDef, arrowDef, 127);
	SDL_memcpy(musicInfo[musicCount].name, name, 49);
	musicCount++;

	return musicCount - 1;
}

int Audio::AddSound(const char* filename)
{
	if (!filename) return -1;
	if (!initialised) return -1;

	Mix_Chunk* mus = Mix_LoadWAV(filename);
	if (!mus)
	{
		sprintf(errMsg, "AddSound(): Cannot load sound file '%s'.\n", filename);
		EHLog(errMsg);
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

void Audio::PlayMusic(int index)
{
	if (!initialised) return;
	if (index < 0 || index >= musicCapacity) return;
	if (!musicInfo[index].music) return;

	StopMusic();
	Mix_PlayMusic(musicInfo[index].music, 0);
	currentMusic = index;
}

void Audio::PauseMusic()
{
	if (!initialised) return;
	if (currentMusic > -1)
	{
		Mix_PauseMusic();
	}
}

void Audio::RestartMusic()
{
	if (!initialised) return;
	if (currentMusic > -1)
	{
		Mix_RewindMusic();
		Mix_ResumeMusic();
	}
}

void  Audio::StopMusic()
{
	if (currentMusic > -1)
	{
		Mix_HaltMusic();
		currentMusic = -1;
	}
}

int Audio::GetInitialised()
{
	return initialised;
}

void Audio::PlaySound(int index)
{
	if (!initialised) return;
	if (index < 0 || index >= chunkCapacity) return;
	if (!chunks[index]) return;
	Mix_PlayChannel(-1, chunks[index], 0);
}

Audio::~Audio()
{
	StopMusic();

	PurgeMusic();
	PurgeSound();

	if(musicInfo)
		free(musicInfo);
	
	if (chunks)
		free(chunks);

	Mix_CloseAudio();
	Mix_Quit();
}

int Audio::LoadMusicList(const char* filename, Audio* audio)
{
	int songCount;
	int i;
	FILE *file;
	char fname[256];
	char name[50];
	char arrowDef[128];
	double length;

	if (!audio || !filename) return 0;

	// <filename> <name> <length> <arrowDefFile>
	file = fopen(filename, "r");
	if (!file)
	{
		sprintf(errMsg, "LoadMusicList(): Cannot load music list '%s'.\n", filename);
		EHLog(errMsg);
		return 0;
	}

	// get number of songs
	songCount=0;
	fscanf(file, "%d", &songCount);

	for (i = 0; i < songCount; i++)
	{
		fscanf(file, "%s %s %lf %s", fname, name, &length, arrowDef);
		audio->AddMusic(fname, name, length, arrowDef);
	}

	fclose(file);
	return 1;
}

MusicInformation* Audio::GetMusicInformation(int index)
{
	if (!initialised) return NULL;
	if (index<0 || index>=musicCount) return NULL;

	return &musicInfo[index];
}

MusicInformation* Audio::GetCurrentMusic()
{
	return GetMusicInformation(currentMusic);
}

int Audio::GetMusicCount() {
	return musicCount;
}