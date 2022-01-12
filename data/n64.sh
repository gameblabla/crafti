#/!bin/sh
ffmpeg -i "explode.wav" -f s16be -acodec pcm_s16be -ar 11025 explode.raw
ffmpeg -i "put.wav" -f s16be -acodec pcm_s16be -ar 11025 put.raw
ffmpeg -i "remove.wav" -f s16be -acodec pcm_s16be -ar 11025 remove.raw
