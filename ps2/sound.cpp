// STD C:
#include <stdlib.h>

// PS2DEV SDK:
#include <sifrpc.h>
#include <loadfile.h>
#include <audsrv.h>

#include "put.h"
#include "explode.h"
#include "remove.h"

extern void *audsrv_irx;
extern int size_audsrv_irx;

static audsrv_adpcm_t sample[3];

void Init_Sound()
{
	SifLoadModule("rom0:LIBSD", 0, NULL);

	SifExecModuleBuffer(&audsrv_irx, size_audsrv_irx, 0, NULL, NULL);

	audsrv_init();
	
	audsrv_adpcm_init();
	audsrv_adpcm_set_volume(0, MAX_VOLUME);
	audsrv_adpcm_set_volume(1, MAX_VOLUME);
	audsrv_adpcm_set_volume(2, MAX_VOLUME);

	audsrv_load_adpcm(&sample[0], (void*)put_adpcm_data, put_adpcm_data_len);
	audsrv_load_adpcm(&sample[1], (void*)remove_adpcm_data, remove_adpcm_data_len);
	audsrv_load_adpcm(&sample[2], (void*)explode_adpcm_data, explode_adpcm_data_len);
}

void Play_SFX(unsigned char i)
{
	audsrv_ch_play_adpcm(i, &sample[i]);
} 

void Close_Sound()
{
	if (audsrv_stop_audio() != 0)
	{
		return;
	}
} 
