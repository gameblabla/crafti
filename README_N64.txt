Crafti for the N64, 1.0 alpha
=========================================================

Ported by Gameblabla

Initially made for the TI Nspire by Vogtinator,
i've ported the game to the Sega Dreamcast and then eventually the PS2 and OG Xbox.
Now, it's also available for the N64.

I want to be very clear : 
if you want to enjoy/play this, you will need an expansion pack.
Otherwise, you will encounter a lot of freezes and crashes.
It appears to be a memory leak of sorts that is fixed by the expansion pack (Donkey Kong 64 vibes much ?) but i still wanted
to release something anyway.

Comparison
===========

All of these versions are built with open source SDKs.

Dreamcast/NAOMI
- Runs at 240p.
- Framerate is around 20-30 FPS
- Used to run like crap but runs pretty OK now.
- Has music and sound effects. (Sound effects are ADPCM compressed except on NAOMI)

Sony PS2
- Runs at either 480i or 480p if user holds triangle button.
- Framerate is 8-15 FPS
- 240p runs much better than even the Dreamcast port but it was very inconsistent so for now i forced it to 480i/480p.
- No music but sound effects (ADPCM)

Original Xbox
- Runs at 480i/480p. Codebase also has support for 720p but this isn't enabled for now.
- Framerate is a smooth 60 FPS
- No sound at all (Audio support in NXDK is pretty poor but i will see later if i can use mikmod or something smaller on it)

Nintendo 64
- Resolution is set to 256x240 but internal resolution is 256x224 to try to save some memory and CPU cycles.
- Framerate can go anywhere from 6 to 20 FPS. Average is 11 - 13 FPS.
- No sound at all as it would just crash on real hardware. (even with an expansion pack. Oddly seems to work fine on emulators like Cen64)


Development details
===================

First of all, Crafti is entirely software rendered so it won't even attempt to make use of a GPU at all.
It should be possible to plug the 3D API code within nGL itself but that would be a lot more work and in the case of the N64,
i'm not even sure if that's worth it or not.

Initially i was considering using n64chain instead of libdragon.
However libdragon is now in a much better shape and the toolchain now makes use of the N32 ABI as well so it should be plenty fast for my needs.
There's still the problem of newlib not providing a 64-bits memcpy, which is why i took the one from 64Doom to hopefully try to speedup some things.

The most painful part was probably trying to figure out Vogtinator's codebase and the quirks of libdragon.
Fortunately after reading a comment on github about display_lock and display_disp, i was able to understand why it failed.

As for software rendering itself, at first i was doing a memcpy.
However i thought this was wasteful so i started to look for ways to use DMA but i've been told it's not possible to use DMA
to copy one section of RAM to another. (He did say the RSP could do just that but it wasn't ready yet)
Eventually i realized (as i said above about display_lock and all) that i could avoid copying altogether by making a few changes to the
codebase and pointing the screen pointers to the framebuffer.

The improvements in terms of FPS was disappointing (it was more like 1-2 FPS) but it did some quite a lot of memory,
which this game surely needs...

I was initially going with 320x240 for the screen resolution. But it was clear that the game
was struggling so i switched to 256x240 instead. 
That was a bit better (+3 FPS) but now the inventory was all wrong so this did require me to change yet more code in the codebase to handle this cornercase better.
I also realized that my TV was cutting off a lot so i decided to just lower the height to 224 and center it on screen.
Decreasing the vertical resolution is CPU-free and the easier thing to do.
Decrasing the horizontal resolution would have required aligning the buffer, which would negate the benefits from going
to a lower resolution in the first place.

I was not able to get sound to work sadly : it works on Cen64 but on real hardware it just crashes.
I'm assuming there's a memory leak somewhere that i haven't found and it seems GCC has not found it yet either.
Perhaps it is within libdragon itself...

Controls 
=========

Ingame
======

L/R = Select slot
Start = Bloc selection menu
A = Place block
B = Remove Block
Z = Jump

Left stick/dpad = move around
C-stick = look around
