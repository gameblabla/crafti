#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>
#include "audio.h"

//#define SOUND_SUPPORT 1

audio_t *g_audio = nullptr;

void Init_Sound()
{
	#ifdef SOUND_SUPPORT
	g_audio = audio_setup( FREQUENCY_11KHZ, 3 );
    audio_write_silence();
	audio_load_pcm(g_audio, 0, "/put.raw");
	audio_load_pcm(g_audio, 1, "/remove.raw");
	audio_load_pcm(g_audio, 2, "/explode.raw");
	#endif
} 

void Play_SFX(unsigned char i)
{
	#ifdef SOUND_SUPPORT
	audio_play_sfx( g_audio, i);
	#endif
} 

void Close_Sound()
{
	#ifdef SOUND_SUPPORT
	audio_free( g_audio );
	g_audio = NULL;
	#endif
} 

void Update_Sound()
{
	#ifdef SOUND_SUPPORT
	audio_tick( g_audio );
	#endif
}
