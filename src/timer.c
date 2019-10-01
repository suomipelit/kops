/* timer.c
 * Copyright 1996,2001,2003 Jetro Lauha.
 *
 * $Id: timer.c,v 1.1 2003/04/15 18:15:49 tonic Exp $
 * $Revision: 1.1 $
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "wport.h"

#ifdef __WATCOMC__
/* old watcom dos code */

void(__interrupt __far *biostimerhandler)();
volatile long int clockticks, counter;

void __interrupt __far timerhandler()
{
    if (gamepause == 0)
        framecounter++;

    clockticks += counter;

    if (clockticks >= 0x10000) {
        clockticks -= 0x10000;
        biostimerhandler();
    } else
        outp(0x20, 0x20);
}

void settimer(short frequency)
{
    framecounter = 0;
    clockticks = 0;
    counter = 0x1234dd / frequency;
    biostimerhandler = _dos_getvect(8);
    _dos_setvect(8, timerhandler);
    outp(0x43, 0x34);
    outp(0x40, counter % 256);
    outp(0x40, counter / 256);
}

void freetimer()
{
    outp(0x43, 0x34);
    outp(0x40, 0);
    outp(0x40, 0);
    _dos_setvect(8, biostimerhandler);
}

#else

static SDL_TimerID timerid;

Uint32 timerhandler(Uint32 interval, void *param)
{
    if (gamepause == 0) {
        framecounter++;
    }
    return interval;
}

void settimer(short frequency)
{
    framecounter = 0;
    timerid = SDL_AddTimer(1000 / frequency, timerhandler, NULL);
}

void freetimer()
{
    SDL_RemoveTimer(timerid);
}

#endif
