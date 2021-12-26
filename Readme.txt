=========================

nCrafti (Dreamcast port)

=========================
Port to Dreamcast by gameblabla

Crafti is a software-rendered minecraft clone by Vogtinator.
It was originally designed for the TI Nspire CX, a very limited calculator with a low clocked CPU at 132 Mhz and a slow screen.
It had tons of RAM though (64 Mb of RAM), unlike the Dreamcast... (16MB)

The Dreamcast port is based on my GCW0 port but it does not use SDL, direct framebuffer access instead. 
This is entirely CPU-rendered, it does not use the PowerVR GPU. (It could run a lot smoothly using the PVR)
The DC port is twice as fast as the Nspire port.
During development, i encountered RAM issues so it's stuck at the low resolution of 320x240.
(Rendering the game at 640x480 is very slow and it crashes after a while, not enough ram...)

The game itself has no monsters or day/night cycle but you do get lots of blocks to play with.
(Including Redstone and TNT blocks !)

I hope i can get this merged upstream someday,
meanwhile hope you'll have fun with it on your dreamcast !
(So far, it's the only Minecraft clone on the console)

==========

Controls

==========

Dpad = Move character
Stick = Camera
A = Put block
B = Remove block
X = Jump

L/R = Switch inventory
Start = Inventory menu