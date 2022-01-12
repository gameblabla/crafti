#!/bin/sh
ffmpeg -i terrain.png -c:v rawvideo -pix_fmt rgb555le rl.rgb
bin2c -w rl.rgb t.h terrain_data
