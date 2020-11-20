#include "SoundSystem.h"

#include <SDL.h>
#include <SDL_mixer.h>

#include "FileSystemUtils.h"
#include <windows.h>

char* messagesiguess;

MusicTrack::MusicTrack(const char* fileName)
{
	m_music = Mix_LoadMUS(fileName);
	m_isValid = true;
	if(m_music == NULL)
	{
		fprintf(stderr, "Unable to load Ogg Music file: %s\n", Mix_GetError());
		m_isValid = false;
	}
}

MusicTrack::MusicTrack(SDL_RWops *rw)
{
	m_music = Mix_LoadMUS_RW(rw, 0);
	m_isValid = true;
	if(m_music == NULL)
	{
		fprintf(stderr, "Unable to load Magic Binary Music file: %s\n", Mix_GetError());
		m_isValid = false;
	}
}

SoundTrack::SoundTrack(const char* fileName)
{
	sound = NULL;

	unsigned char *mem;
	size_t length = 0;
	FILESYSTEM_loadFileToMemory(fileName, &mem, &length);
	SDL_RWops *fileIn = SDL_RWFromMem(mem, length);
	sound = Mix_LoadWAV_RW(fileIn, 1);
	if (length)
	{
		FILESYSTEM_freeMemory(&mem);
	}

	if (sound == NULL)
	{
		//sprintf(messagesiguess, "Unable to load WAV file: %s\n", Mix_GetError());
		//utputDebugStringA(messagesiguess);
	}
}

SoundSystem::SoundSystem()
{
	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 1024;
	//this appears to be getting called before sdl init for some reason
	//look at call stack
	if (SDL_WasInit(SDL_INIT_AUDIO) == 0)
	{
		OutputDebugStringA("audio no init");
		/*if (SDL_Init(SDL_INIT_AUDIO) != 0)
		{
			OutputDebugStringA(SDL_GetError());
		}*/
	}
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
	{
		OutputDebugStringA(Mix_GetError());
		//sprintf(messagesiguess, "Unable to initialize audio: %s\n", Mix_GetError());
		//OutputDebugStringA(messagesiguess);
		SDL_assert(0 && "Unable to initialize audio!");
	}
}

void SoundSystem::playMusic(MusicTrack* music)
{
	if(!music->m_isValid)
	{
		fprintf(stderr, "Invalid mix specified: %s\n", Mix_GetError());
	}
	if(Mix_PlayMusic(music->m_music, 0) == -1)
	{
		fprintf(stderr, "Unable to play Ogg file: %s\n", Mix_GetError());
	}
}
