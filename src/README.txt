
Source code of the game KOPS.
Copyright 1996-2003 Jetro Lauha.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

An exception to the GPL license is that this game can link to
non-GPL-licensed FMOD sound library.

------------------------------------------------------------------------------

Note that the game contains old legacy code and quick and dirty wrapper code
to make the game compile on a more modern platform. KOPS was originally made
back in year 1996 as a quick little game project, and that version used
even older graphics library code which was ported from Pascal sources to C.
Due to these historical reasons the game code isn't very clear or well
designed in some parts.

Antti Tapaninen did the original Linux port and then later Sami Mäkinen-Okubo
worked on putting some AI code to the game. He didn't have time to fully
finish the AI implementation, but the base code is still there.
They also cleaned up the source a bit.

In addition to the actual game code, the source directory contains also
source for the level converter utility "conv", and AI .kb parser utility
kb-parser.

VisualC directory contains workspace and projects for the game code and
conv utility, but you may have to compile the kb-parser with gcc (you can
use e.g. cygwin if you want to run it on win32).

The VC projects point to SDL, zlib and fmod libraries in directories
..\..\..\..\LIBS\SDL\SDL, ..\..\..\..\LIBS\zlib and ..\..\..\..\LIBS\fmod.
You have to install the libraries to equivalent directory or alternatively
edit the project files.

The game has been tested with SDL 1.2.5, zlib 1.1.4 and FMOD 3.62.

KOPS home page URL is http://iki.fi/jetro/kops/


--
$Id: README.txt,v 1.1 2003/04/15 18:15:49 tonic Exp $
