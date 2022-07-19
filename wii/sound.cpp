#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <ogcsys.h>
#include <ogc/lwp_watchdog.h>

#include <gxflux/gfx.h>
#include <gxflux/gfx_con.h>

#include <assert.h>
#include <signal.h>

#include <asndlib.h>

#include <aesndlib.h>
#include <gcmodplay.h>

#include "technique_mod.h"
#include "explode.h"
#include "put.h"
#include "remove.h"

static MODPlay play;

void Init_Sound()
{
	//AESND_Init();
	ASND_Init();
	ASND_Pause(0);
	
	//
	/*MODPlay_Init(&play);
	MODPlay_SetMOD(&play,technique_mod);
	MODPlay_Start(&play);*/
} 

void Play_SFX(unsigned char i)
{
	switch(i)
	{
		case 0:
			ASND_SetVoice(8, VOICE_STEREO_16BIT, 44100, 0, (void *)put_pcm, put_pcm_size, MAX_VOLUME, MAX_VOLUME, NULL);
		break;
		case 1:
			ASND_SetVoice(9, VOICE_STEREO_16BIT, 44100, 0, (void *)remove_pcm, remove_pcm_size, MAX_VOLUME, MAX_VOLUME, NULL);
		break;
		case 2:
			ASND_SetVoice(10, VOICE_STEREO_16BIT, 44100, 0, (void *)explode_pcm, explode_pcm_size, MAX_VOLUME, MAX_VOLUME, NULL);
		break;
	}

} 

void Close_Sound()
{
	/*MODPlay_Stop(&play);
	MODPlay_Unload(&play);*/
} 
