#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>

//#define SOUND_SUPPORT 1

#define CHANNEL_MUSIC   3

wav64_t snd_sfx[3];

void Init_Sound()
{
	#ifdef SOUND_SUPPORT
	audio_init(44100, 5);
	mixer_init(8);  // Initialize up to 16 channels
	wav64_open(&snd_sfx[0], "put.wav64");
	wav64_open(&snd_sfx[1], "remove.wav64");
	wav64_open(&snd_sfx[2], "explode.wav64");
	wav64_set_loop(&snd_sfx[0], false);
	wav64_set_loop(&snd_sfx[1], false);
	wav64_set_loop(&snd_sfx[2], false);
	#endif
} 

void Play_SFX(unsigned char i)
{
	#ifdef SOUND_SUPPORT
	mixer_ch_play(i, &snd_sfx[i].wave);
	mixer_ch_set_vol(i, 1.0f, 1.0f);
	#endif
} 

void Close_Sound()
{
	#ifdef SOUND_SUPPORT
	#endif
} 

void Update_Sound()
{
	#ifdef SOUND_SUPPORT
		// Check whether one audio buffer is ready, otherwise wait for next
		// frame to perform mixing.
		if (audio_can_write()) {    	
			short *buf = audio_write_begin();
			mixer_poll(buf, audio_get_buffer_length());
			audio_write_end();
		}
	#endif
}
