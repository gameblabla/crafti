/**
 * FlappyBird-N64 - audio.c
 *
 * Copyright 2017, Christopher Bonhage
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libdragon.h>
#include "audio.h"


audio_t *audio_setup(const uint16_t sample_rate, const uint8_t buffers)
{
    /* Start up the audio subsystem */
    audio_init( sample_rate, buffers );
    const uint16_t buffer_length = audio_get_buffer_length();
    int16_t *buffer = malloc( buffer_length * STEREO_PCM_SAMPLE_SIZE );
    audio_t *audio = malloc( sizeof( audio_t ) );
    audio->sample_rate = sample_rate;
    audio->frames = buffer_length << 1;
    audio->buffer = buffer;
    
    return audio;
}

void audio_load_pcm(audio_t *audio, uint8_t number, const char* path)
{
    audio->sfx_cache[number] = read_dfs_pcm_sound( path,  audio->sample_rate, 1 );

    // Setup the sound effects channels
    audio->channels[number].cursor = 0;
	audio->channels[number].sfx = NULL;
}

void audio_free_pcm(audio_t *audio, uint8_t number)
{
	if (audio->sfx_cache[number] != NULL)
	{
		if (audio->sfx_cache[number]->data != NULL)
		{
			free( audio->sfx_cache[number]->data );
			audio->sfx_cache[number]->data = NULL;
		}
		
        free( audio->sfx_cache[number] );
		audio->sfx_cache[number] = NULL;
		
		audio->channels[number].cursor = 0;
		audio->channels[number].looping = 2;
		if (audio->channels[number].sfx != NULL)
		{
			audio->channels[number].sfx = NULL;
		}
	}
}

void audio_free(audio_t *audio)
{
    /* Clear the sound effects cache */
    for (uint8_t i = 0; i < SFX_NUM_SOUNDS; i++)
    {
        if (audio->sfx_cache[i]->data != NULL)
        {
			free( audio->sfx_cache[i]->data );
		}
		
        if (audio->sfx_cache[i] != NULL)
        {
			free( audio->sfx_cache[i] );
			audio->sfx_cache[i] = NULL;
		}
        
    }
    /* Clear sound effects pointers from playback channels */
    for (uint8_t i = 0; i < SFX_NUM_CHANNELS; i++)
    {
		if (audio->channels[i].sfx != NULL)
		{
			audio->channels[i].sfx = NULL;
		}
    }
    /* Shut down the audio subsystem */
    free( audio->buffer );
    free( audio );
    audio_close();
}

inline static int16_t mix_pcm_samples(int32_t mix, uint8_t num)
{
    return (num > 1) ? mix / num : mix;
}

void audio_tick(audio_t *audio)
{
    if ( audio != NULL && audio_can_write() )
    {
        sfx_channel_t channel;
        pcm_sound_t *sfx;
        int32_t left_mix, right_mix;
        uint8_t left_num, right_num;
        /* Fill the audio buffer with stereo sample frames */
        for (uint16_t frame = 0; frame < audio->frames;
             left_mix = left_num = right_mix = right_num = 0)
        {
            /* Accumulate all currently playing sound effects samples */
            for (uint8_t i = 0; i < SFX_NUM_CHANNELS; i++)
            {
                channel = audio->channels[i];
                sfx = channel.sfx;
                if ( sfx != NULL && channel.cursor < sfx->samples )
                {
                    left_mix += sfx->data[channel.cursor++];
                    left_num++;
                    /* Play mono sound effects in both speakers */
                    if ( sfx->channels == 1 )
                    {
                        right_mix += sfx->data[channel.cursor - 1];
                        right_num++;
                    }
                    /* Play stereo sound effects in separate speakers */
                    else if ( channel.cursor < sfx->samples )
                    {
                        right_mix += sfx->data[channel.cursor++];
                        right_num++;
                    }
                    /* Reset channels that have finished playing */
                    if ( channel.cursor >= sfx->samples)
                    {
                        channel.cursor = 0;
                        if (channel.looping == 0) channel.sfx = NULL;
                    }
                }
                audio->channels[i] = channel;
            }
            /* Mix down all of the samples as an average */
            audio->buffer[frame++] = mix_pcm_samples( left_mix, left_num );
            audio->buffer[frame++] = mix_pcm_samples( right_mix, right_num );
        }
        audio_write( audio->buffer );
    }
}

void audio_play_sfx(audio_t *audio, const uint8_t sfx_sound)
{
    if ( audio != NULL )
    {
        if ( audio->sfx_cache[sfx_sound] != NULL )
        {
			/* First channel is reserved for music. Maybe in the future, this can be expanded to 2
			 * for stereo audio. (or heck, even 4, 5.. for surrond or Mod-based music)*/
            for (uint8_t i = 1; i < SFX_NUM_CHANNELS; i++)
            {
                if ( audio->channels[i].sfx == NULL )
                {
					audio->channels[i].looping = 0;
                    audio->channels[i].sfx = audio->sfx_cache[sfx_sound];
                    audio->channels[i].cursor = 0;
                    break;
                }
            }
        }
    }
}

void audio_play_music(audio_t *audio, const uint8_t sfx_sound, const uint8_t looping)
{
    if ( audio != NULL )
    {
        if ( audio->sfx_cache[sfx_sound] != NULL )
        {
			audio->channels[0].looping = looping;
			audio->channels[0].sfx = audio->sfx_cache[sfx_sound];
			audio->channels[0].cursor = 0;
        }
    }
}

pcm_sound_t *read_dfs_pcm_sound(const char *file, uint16_t sample_rate, uint8_t channels)
{
    int fp = dfs_open( file );
    const uint32_t size = dfs_size( fp );
    int16_t *data = malloc( size );
    dfs_read( data, 1, size, fp );
    dfs_close( fp );
    uint32_t frames = size / MONO_PCM_SAMPLE_SIZE;
    pcm_sound_t *sound = malloc( sizeof( pcm_sound_t ) );
    sound->sample_rate = sample_rate;
    sound->channels = channels;
    sound->frames = frames;
    sound->samples = frames / channels;
    sound->data = data;
    return sound;
}
