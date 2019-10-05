KOPS
====

:point_right: **The original README.txt can be found as
[src/README.txt](./src/README.txt).**

This is a classic 90s 2D cave flying shooter game, ported to SDL2. It
runs on at least macOS, Linux and Windows.

The port is maintained by the [Suomipelit][suomipelit-gh]
organization, whose mission is to archive, port, and maintain classic
Finnish freeware and shareware games.  Feel free to [join the
Suomipelit Slack][suomipelit-slack] too!

:point_right: Binary data is currently provided as a separate download
[here at
SourceForge](https://sourceforge.net/projects/kops/files/kops/kops-initial-data/kops-data-20030415.zip/download). Extract
into the working copy directory.

Building from source
--------------------

[![Build
Status](https://travis-ci.org/suomipelit/kops.svg?branch=master)](https://travis-ci.org/suomipelit/kops)

**Requirements:**

- CMake
- C++ compiler: At least gcc, clang and Visual Studio are supported
- Libraries: SDL2, SDL2_mixer, zlib
  - On macOS, you can install these with Homebrew. `brew install sdl2
    sdl2_mixer sdl2_image zlib`

**Note** While KOPS port is fully playable with official SDL2_Mixer,
the music track looping doesn't work correctly. While you probably
won't even notice the difference unless you know what to listen for,
it can be fixed by compiling SDL2_Mixer by yourself and changing
`settings.mLoopCount = 0;` to `settings.mLoopCount = -1;` in
`music_modplug.c`.

**Building:**

```shell
cd bin
cmake ..
cmake --build .
```
