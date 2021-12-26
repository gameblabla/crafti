/*#include <SDL_mixer.h>

static Mix_Music* music;
static Mix_Chunk* sfx[4];*/

#include <kos.h>
#include <string.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>

//int  SoundTrack;
sfxhnd_t SEPool[6];

void Init_Sound()
{
	snd_stream_init();
	//sndoggvorbis_init();
		  
#ifdef NAOMI
	SEPool[0] = snd_sfx_load("/rd/put.wav"); 
	SEPool[1] = snd_sfx_load("/rd/remove.wav"); 
	SEPool[2] = snd_sfx_load("/rd/explode.wav");
#else
	SEPool[0] = snd_sfx_load("/cd/put.wav"); 
	SEPool[1] = snd_sfx_load("/cd/remove.wav"); 
	SEPool[2] = snd_sfx_load("/cd/explode.wav");
	cdrom_cdda_play(1, 1, 0, CDDA_TRACKS);
	spu_cdda_volume(15, 15);
	spu_cdda_pan(15, 15);
#endif
	//sndoggvorbis_start("/cd/music.ogg",0);*/
} 

void Play_SFX(unsigned char i)
{
	snd_sfx_play(SEPool[i], 255, 0x80);
} 

void Close_Sound()
{
	//sndoggvorbis_stop();
} 
