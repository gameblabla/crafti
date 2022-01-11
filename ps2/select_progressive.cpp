//  ____     ___ |    / _____ _____
// |  __    |    |___/    |     |
// |___| ___|    |    \ __|__   |     gsKit Open Source Project.
// ----------------------------------------------------------------------
// Copyright 2004 - Chris "Neovanglist" Gilbert <Neovanglist@LainOS.org>
// Licenced under Academic Free License version 2.0
// Review gsKit README & LICENSE files for further details.
//
// text_draw.c - Example demonstrating gsKit texture operation.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <gsKit.h>
#include <dmaKit.h>
#include <malloc.h>
#include <gsToolkit.h>
#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <stdio.h>
#include <libpad.h>

#include "instructions_texture.h"

extern struct padButtonStatus buttons;
extern u32 paddata;
extern u32 old_pad;
extern u32 new_pad;

int select_resolution()
{
	/*gsGlobal *glob_inst;
	GSTEXTURE text_draw;
	int exit = 0;
	int mode = 0;
	int ret;
	
	glob_inst = gsKit_init_global();
	
	glob_inst->Mode = gsKit_check_rom();
	glob_inst->Interlace = GS_INTERLACED;
	glob_inst->Field = GS_FIELD;
	glob_inst->Width = 640;
	glob_inst->Height = 480;

	glob_inst->PSM  = GS_PSM_CT16;
	glob_inst->PSMZ = GS_PSMZ_16;
	glob_inst->Dithering = GS_SETTING_OFF;
	glob_inst->DoubleBuffering = GS_SETTING_OFF;
	glob_inst->ZBuffering = GS_SETTING_OFF;
	glob_inst->StartX=0;
	glob_inst->StartY=0;

	gsKit_init_screen(glob_inst);
	gsKit_mode_switch(glob_inst, GS_PERSISTENT);

	text_draw.Width = 640;
	text_draw.Height = 480;
	text_draw.PSM = GS_PSM_CT16;
	text_draw.Filter = GS_FILTER_NEAREST;
	text_draw.Mem = (u32 *)texture;
	text_draw.Vram = gsKit_vram_alloc(glob_inst, gsKit_texture_size(text_draw.Width, text_draw.Height, text_draw.PSM), GSKIT_ALLOC_USERBUFFER);
	
	gsKit_texture_upload(glob_inst, &text_draw);

	while(exit == 0)
	{
		paddata = 0xffff ^ buttons.btns;
		
		new_pad = paddata & ~old_pad;
		old_pad = paddata;
		ret = padRead(0, 0, &buttons); // port, slot, buttons
		if (ret != 0)
		{
			if (new_pad & PAD_CROSS)
			{
				mode = 0;
				exit = 1;
			}
			else if (new_pad & PAD_CIRCLE)
			{
				mode = 1;
				exit = 1;
			}
		}

		gsKit_setup_tbw(&text_draw);
		gsKit_prim_sprite_texture(glob_inst, &text_draw, 0.0f,  // X1
				0.0f,  // Y2
				0.0f,  // U1
				0.0f,  // V1
				glob_inst->Width, // X2
				glob_inst->Height, // Y2
				glob_inst->Width, // U2
				glob_inst->Height, // V2
				0,
		0x80808080);
		gsKit_queue_exec(glob_inst);
		gsKit_sync_flip(glob_inst);
	}
	gsKit_deinit_global(glob_inst);*/
	int i, ret, mode = 0;
	for(i=0;i<60;i++)
	{
		paddata = 0xffff ^ buttons.btns;
		
		new_pad = paddata & ~old_pad;
		old_pad = paddata;
		ret = padRead(0, 0, &buttons); // port, slot, buttons
		if (ret != 0)
		{
			if (new_pad & PAD_TRIANGLE)
			{
				mode = 1;
				break;
			}
		}
	}

	return mode;
}
