#!/bin/sh
ffmpeg -i put.wav -f s16be -acodec pcm_s16be  put.pcm
ffmpeg -i remove.wav -f s16be -acodec pcm_s16be remove.pcm
ffmpeg -i explode.wav -f s16be -acodec pcm_s16be explode.pcm
