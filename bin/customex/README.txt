KOPS customization


This text file describes how to create custom external levels and
custom external music files, and how to use them.


External levels can be created with the conv-utility.
Look in ex_map.pcx and ex_back.pcx for example level graphics.

Example:
conv customex/ex_map.pcx customex/ex_back.pcx example.kop

Creates example.kop-level from ex_map.pcx and ex_back.pcx in
customex-directory. Palette which will be used in the level is
loaded from ex_map.pcx (first 128 colors with indices 0-127 can
be used). Make sure that the game main data file kops.jfl is
available in the same working directory.


Supported external music file types are the module formats what
FMOD recognizes (S3M is used in the original version).

External music init file must be generated for the external
music set. External music can be used by using /m parameter
with the game. Look in customex/example.s3m and customex/example.kmu
for example external music set.

Running the game with command /mexample.kmu starts the game using
the example.kmu external music set instead of the default music.


--
$Id: README.txt,v 1.1 2003/04/15 17:26:32 tonic Exp $
