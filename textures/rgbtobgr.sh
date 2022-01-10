#!/bin/sh
ffmpeg -i terrain.png -c:v rawvideo -pix_fmt rgb565le rl.rgb
bin2c -w rl.rgb t.h terrain_data
