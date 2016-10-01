#include <SDL_mixer.h>

static Mix_Music* music;
static Mix_Chunk* sfx[4];

void Init_Sound()
{
	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024);
	Mix_AllocateChannels(2);
	music = Mix_LoadMUS("data/music.ogg");
	Mix_PlayMusic(music, -1);
	sfx[0] = Mix_LoadWAV("data/put.wav");
	sfx[1] = Mix_LoadWAV("data/remove.wav");
	sfx[2] = Mix_LoadWAV("data/explode.wav");
} 

void Play_SFX(unsigned char i)
{
	Mix_PlayChannel(-1, sfx[i], 0);
} 

void Close_Sound()
{
	if (music)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
	}
} 
