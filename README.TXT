Crafti for Dreamcast by gameblabla
====================================

This is a port of Vogtinator's Crafti for the Dreamcast.
A port for the GCW-Zero was also made a while back.

This is basically a simple minecraft clone that is fully software rendered,
although it does not have creepers.

The game used to support the SD card adapter but due to memory constraints,
CDDA music has been implemented. As a result, it's only
possible to play this with a GDEMU or by burning a game disc.
It's still possible to play the game but you won't get game music (just like the previous version).

Changelog
========

1.1 :
- Speed improvements (backport changes from Vogtinator's new branch + using sq block copies)
- CDDA Music

Overall, it should play quite a bit better.
This has been tested on a GDEMU and is now provided as an Alcohol 120% MDS/MDF image file.
This is because CDI4DC cannot generate Audio/Data dumps with CDDA tracks and i don't want to workaround this
by using DiscJuggler.
