#!/bin/sh
ffmpeg -i real.png -c:v rawvideo -pix_fmt bgr555le rl.rgb
bin2c -w rl.rgb instructions_texture.h texture
rm rl.rgb
