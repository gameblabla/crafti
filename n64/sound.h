#ifndef _SOUND_DEFINE
#define _SOUND_DEFINE

#ifdef __cplusplus
extern "C"
{
#endif

void Init_Sound();
void Play_SFX(unsigned char i); 
void Close_Sound();
void Update_Sound();

#ifdef __cplusplus
}
#endif

#endif
