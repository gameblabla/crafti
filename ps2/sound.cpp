/*#include <mikmod.h>

MODULE *mf = NULL;
SAMPLE *sf[2];*/

void Init_Sound()
{
	/*MikMod_RegisterAllDrivers();
	MikMod_RegisterAllLoaders();
	md_mode = DMODE_16BITS|DMODE_STEREO|DMODE_SOFT_SNDFX|DMODE_SOFT_MUSIC; 
	md_reverb = 0;
	md_pansep = 128;
	MikMod_Init("");
	MikMod_SetNumVoices(-1, 8);
	MikMod_EnableOutput();
	
	sf[0] = Sample_Load("mass:/put.wav");
	sf[1] = Sample_Load("mass:/remove.wav");
	sf[2] = Sample_Load("mass:/explode.wav");*/
}

void Play_SFX(unsigned char i)
{
	//Sample_Play(sf[i],0,0);
} 

void Close_Sound()
{
	/*Player_Stop();
	Player_Free(mf);
	MikMod_Exit();*/
} 
