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
#include "timer.h"

int timer_rate = TIMERRATE;
static SDL_TimerID timerid;

Uint32 timerhandler(Uint32 interval, void *param) {
    if (gamepause == 0) {
        framecounter++;
    }
    return interval;
}

void settimer(int frequency) {
    if (timerid) {
        freetimer();
    }
    // Above 400hz things start to be silly...
    frequency = (frequency < 15 ? 15 : (frequency > 400 ? 400 : frequency));
    timer_rate = frequency;
    framecounter = 0;
    timerid = SDL_AddTimer(1000 / frequency, timerhandler, NULL);
    printf("Timer set to %d hz.\n", timer_rate);
}

void freetimer() {
    SDL_RemoveTimer(timerid);
}
