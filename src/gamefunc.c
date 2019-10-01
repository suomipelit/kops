/* gamefunc.c
 * Copyright 1996,2001,2003 Jetro Lauha.
 *
 * $Id: gamefunc.c,v 1.1 2003/04/15 18:15:49 tonic Exp $
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "wport.h"
#include "global.h"

void placeplayer(Uint8 nro)
{
    int err = 1, a, b, x = 0, y = 0;
    while (err > 0) {
        err = 0;
        x = rand() % (levw - SHIPW);
        y = rand() % (levh - SHIPH);
        for (b = 0; b < SHIPH; b++) {
            for (a = 0; a < SHIPW; a++) {
                if (*(level + (y + b) * levw + x + a)) {
                    err++;
                }
            }
        }
    }
    plr[nro].x = x << FIXP;
    plr[nro].y = y << FIXP;
    plr[nro].xp = x;
    plr[nro].yp = y;
}

char hitdetect(int x, int y)
{

#define CHECKHIT(X, Y) ((X) >= 0 && (Y) >= 0 && (X) < levw && (Y) < levh && *(mask + (Y)*levw + (X)) == 0)

    char hit = 0;

    if (CHECKHIT(x + SHIPW / 2, y + SHIPH / 2 + 2) ||
        CHECKHIT(x + SHIPW / 2, y + SHIPH / 2 - 2) ||
        CHECKHIT(x + SHIPW / 2 + 2, y + SHIPH / 2) ||
        CHECKHIT(x + SHIPW / 2 - 2, y + SHIPH / 2)) {
        hit = 1;
    }
#undef CHECKHIT
    /*
      if ((*(mask+(y+SHIPH/2+2)*levw+x+SHIPW/2)==0)||
      (*(mask+(y+SHIPH/2-2)*levw+x+SHIPW/2)==0)||
      (*(mask+(y+SHIPH/2)*levw+x+SHIPW/2+2)==0)||
      (*(mask+(y+SHIPH/2)*levw+x+SHIPW/2-2)==0)) hit=1;
    */
    /*                // pikselintarkka detekti
                      int x,y,o,so;
                      o=y*levw+x;
                      so=(plr[nro].dir>>FIXP)*SHIPSIZE;
                      for (y=0;y<SHIPH;y++)
                      {
                      for (x=0;x<SHIPW;x++)
                      {
                      if ((*(mask+o)==0)&&(*(plr[nro].ship+so))) hit=1;
                      o++; so++;
                      }
                      o+=levw-SHIPW;
                      }
    */
    return (hit);
}

char waterdetect(int x, int y)
{
    char hit = 0, c = 0;
    int detx, dety;

    detx = x + SHIPW / 2;
    dety = y + SHIPH / 2;

    if (detx >= 0 && dety >= 0 && detx < levw && dety < levh) {
        c = *(mask + dety * levw + detx);
    }

    if ((c > 0) && (c < 8)) {
        hit = 1;
    }

    return (hit);
}

Uint8 basedetect(int x, int y)
{
    char hit = 0, c = 0;
    int detx, dety;

    detx = x + SHIPW / 2;
    dety = y + SHIPH / 2 + 5;

    if (detx >= 0 && dety >= 0 && detx < levw && dety < levh) {
        c = *(level + dety * levw + detx);
        if ((c >= 10) && (c <= 14)) {
            hit = 1;
        }
    }
    dety = y + SHIPH / 2 - 5;
    if (detx >= 0 && dety >= 0 && detx < levw && dety < levh) {
        c = *(level + (y + SHIPH / 2 - 5) * levw + x + SHIPW / 2);
        if ((c >= 10) && (c <= 14)) {
            hit = 2;
        }
    }
    return (hit);
}

#if 0
/* API changed, removed old functions. */
char hitdetect(char nro)
{
#define CHECKHIT(x, y) ((x) >= 0 && (y) >= 0 && (x) < levw && (y) < levh && \
                        *(mask + (y)*levw + (x)) == 0)
    char hit = 0;
    if (CHECKHIT(plr[nro].xp + SHIPW / 2, plr[nro].yp + SHIPH / 2 + 2) ||
	CHECKHIT(plr[nro].xp + SHIPW / 2, plr[nro].yp + SHIPH / 2 - 2) ||
	CHECKHIT(plr[nro].xp + SHIPW / 2 + 2, plr[nro].yp + SHIPH / 2) ||
	CHECKHIT(plr[nro].xp + SHIPW / 2 - 2, plr[nro].yp + SHIPH / 2))
	hit = 1;
#undef CHECKHIT
    /*
      if ((*(mask+(plr[nro].yp+SHIPH/2+2)*levw+plr[nro].xp+SHIPW/2)==0)||
      (*(mask+(plr[nro].yp+SHIPH/2-2)*levw+plr[nro].xp+SHIPW/2)==0)||
      (*(mask+(plr[nro].yp+SHIPH/2)*levw+plr[nro].xp+SHIPW/2+2)==0)||
      (*(mask+(plr[nro].yp+SHIPH/2)*levw+plr[nro].xp+SHIPW/2-2)==0)) hit=1;
    */
    /*                // pikselintarkka detekti
                      int x,y,o,so;
                      o=plr[nro].yp*levw+plr[nro].xp;
                      so=(plr[nro].dir>>FIXP)*SHIPSIZE;
                      for (y=0;y<SHIPH;y++)
                      {
                      for (x=0;x<SHIPW;x++)
                      {
                      if ((*(mask+o)==0)&&(*(plr[nro].ship+so))) hit=1;
                      o++; so++;
                      }
                      o+=levw-SHIPW;
                      }
    */
    return (hit);
}

char waterdetect(char nro)
{
    char hit = 0, c = 0;
    int detx, dety;
    detx = plr[nro].xp + SHIPW / 2;
    dety = plr[nro].yp + SHIPH / 2;
    if (detx >= 0 && dety >= 0 && detx < levw && dety < levw)
	c = *(mask + dety * levw + detx);
    if ((c > 0) && (c < 8))
	hit = 1;
    return (hit);
}

Uint8 basedetect(char nro)
{
    char hit = 0, c = 0;
    int detx, dety;

    detx = plr[nro].xp + SHIPW / 2;
    dety = plr[nro].yp + SHIPH / 2 + 5;

    if (detx >= 0 && dety >= 0 && detx < levw && dety < levw)
    {
	c = *(level + dety * levw + detx);
	if ((c >= 10) && (c <= 14))
	    hit = 1;
    }

    dety = plr[nro].yp + SHIPH / 2 - 5;

    if (detx >= 0 && dety >= 0 && detx < levw && dety < levw)
    {
	c = *(level + (plr[nro].yp + SHIPH / 2 - 5) * levw + plr[nro].xp + SHIPW / 2);
	if ((c >= 10) && (c <= 14))
	    hit = 2;
    }

    return (hit);
}
#endif /* OLD API. */

void newammo(Uint8 player, Uint8 type)
{
    int a;
    ammo[nextammo].active = 1;
    ammo[nextammo].x = plr[player].x + ((SHIPW / 2) << FIXP) +
                       cost[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 10 + rand() % (FIXS * 2) - FIXS;
    ammo[nextammo].y = plr[player].y + ((SHIPH / 2) << FIXP) +
                       sint[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 10 + rand() % (FIXS * 2) - FIXS;
    ammo[nextammo].xp = ammo[nextammo].x >> FIXP;
    ammo[nextammo].yp = ammo[nextammo].y >> FIXP;
    ammo[nextammo].oxp = ammo[nextammo].xp;
    ammo[nextammo].oyp = ammo[nextammo].xp;
    ammo[nextammo].hitdist = 8;
    ammo[nextammo].type = type;
    switch (type) {
    case W_TUHNU:
    case W_PEAGUN:
    case W_SLING:
    case W_BIGPEAGUN:
    case W_BIGSLING:
    case W_BLASTGUN:
        ammo[nextammo].xi = cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].xi;
        ammo[nextammo].yi = sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].yi;
        ammo[nextammo].damage = ((type + 1) << FIXP) / 2;
        ammo[nextammo].timeleft = 9999;
        break;
    case W_MINIBLAST:
    case W_BLASTER:
    case W_BIGBLASTR:
    case W_GASBLAST:
    case W_MEGABLAST:
        ammo[nextammo].xi = cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].xi;
        ammo[nextammo].yi = sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].yi;
        ammo[nextammo].damage = (type + 2) << FIXP;
        ammo[nextammo].timeleft = 9999;
        break;
    case W_MINE:
        ammo[nextammo].x = plr[player].x + ((SHIPW / 2) << FIXP) -
                           cost[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 10 - FIXS;
        ammo[nextammo].y = plr[player].y + ((SHIPH / 2) << FIXP) -
                           sint[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 10 - FIXS;
        ammo[nextammo].xi = 0;                /* counter */
        ammo[nextammo].yi = ammo[nextammo].y; /* org y, don't change */
        ammo[nextammo].damage = 16 << FIXP;
        ammo[nextammo].hitdist = 0;
        ammo[nextammo].timeleft = 22500;
        break;
    case W_PLASMA:
        ammo[nextammo].xi = cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 35 / 10 + plr[player].xi;
        ammo[nextammo].yi = sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 35 / 10 + plr[player].yi;
        ammo[nextammo].damage = 0;
        ammo[nextammo].hitdist = 8;
        ammo[nextammo].timeleft = 4510;
        break;
    case W_HEATSEEK:
        ammo[nextammo].xi = cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].xi;
        ammo[nextammo].yi = sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].yi;
        ammo[nextammo].damage = 9 << FIXP;
        ammo[nextammo].timeleft = 9030;
        break;
    case W_CRAWLER:
        ammo[nextammo].x = plr[player].x + ((SHIPW / 2) << FIXP) -
                           cost[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 10 - FIXS;
        ammo[nextammo].y = plr[player].y + ((SHIPH / 2) << FIXP) -
                           sint[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 10 - FIXS;
        ammo[nextammo].xi = -cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 3 + plr[player].xi;
        ammo[nextammo].yi = -sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 3 + plr[player].yi;
        ammo[nextammo].damage = 9 << FIXP;
        ammo[nextammo].timeleft = 22530;
        break;
    case W_SENSORJAM:
    case W_FLIPPER:
    case W_SIGNAL:
        ammo[nextammo].xi = cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].xi;
        ammo[nextammo].yi = sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].yi;
        ammo[nextammo].damage = FIXS / 3;
        ammo[nextammo].timeleft = 9999;
        break;
    case W_TOASTER:
        ammo[nextammo].xi = cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].xi;
        ammo[nextammo].yi = sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].yi;
        ammo[nextammo].damage = FIXS / 2;
        ammo[nextammo].timeleft = 25;
        break;
    case W_GRAVFLOW:
        ammo[nextammo].xi = cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].xi;
        ammo[nextammo].yi = sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].yi;
        ammo[nextammo].damage = FIXS / 4;
        ammo[nextammo].timeleft = 25;
        break;
    case W_LASH:
        ammo[nextammo].xi = cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].xi;
        ammo[nextammo].yi = sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].yi;
        ammo[nextammo].damage = 5 << FIXP;
        ammo[nextammo].timeleft = 9999;
        break;
    case W_MORNSTAR:
        ammo[nextammo].xi = cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].xi;
        ammo[nextammo].yi = sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].yi;
        ammo[nextammo].damage = 8 << FIXP;
        ammo[nextammo].timeleft = 9999;
        break;
    case W_REARSLING:
        ammo[nextammo].x = plr[player].x + ((SHIPW / 2) << FIXP) -
                           cost[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 10 - FIXS;
        ammo[nextammo].y = plr[player].y + ((SHIPH / 2) << FIXP) -
                           sint[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 10 - FIXS;
        ammo[nextammo].xi = -cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 3 + plr[player].xi;
        ammo[nextammo].yi = -sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 3 + plr[player].yi;
        ammo[nextammo].damage = 3 << FIXP;
        ammo[nextammo].timeleft = 9999;
        break;
    case W_BOMB:
        ammo[nextammo].x = plr[player].x + ((SHIPW / 2) << FIXP) -
                           cost[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 12 - FIXS;
        ammo[nextammo].y = plr[player].y + ((SHIPH / 2) << FIXP) -
                           sint[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 12 - FIXS;
        ammo[nextammo].xi = -cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] + plr[player].xi / 2;
        ammo[nextammo].yi = -sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] + plr[player].yi / 2;
        ammo[nextammo].damage = 18 << FIXP;
        ammo[nextammo].timeleft = 9999;
        break;
    case W_PYROTECH:
        ammo[nextammo].x = plr[player].x + ((SHIPW / 2) << FIXP) -
                           cost[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 12 - FIXS;
        ammo[nextammo].y = plr[player].y + ((SHIPH / 2) << FIXP) -
                           sint[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 12 - FIXS;
        ammo[nextammo].xi = -cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] + plr[player].xi / 2;
        ammo[nextammo].yi = -sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] + plr[player].yi / 2;
        ammo[nextammo].damage = 5 << FIXP;
        ammo[nextammo].timeleft = 9999;
        break;
    case W_TOXICWAST:
        ammo[nextammo].x = plr[player].x + ((SHIPW / 2) << FIXP) -
                           cost[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 12 - FIXS;
        ammo[nextammo].y = plr[player].y + ((SHIPH / 2) << FIXP) -
                           sint[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 12 - FIXS;
        ammo[nextammo].xi = -cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] + plr[player].xi / 2;
        ammo[nextammo].yi = -sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] + plr[player].yi / 2;
        ammo[nextammo].damage = 5 << FIXP;
        ammo[nextammo].timeleft = 9999;
        break;
    case W_RINGFIRE:
        for (a = 0; a < 32; a++) {
            ammo[nextammo].active = 1;
            ammo[nextammo].x = plr[player].x + ((SHIPW / 2) << FIXP) + 12 * cost[a * SCT / 32];
            ammo[nextammo].y = plr[player].y + ((SHIPH / 2) << FIXP) + 12 * sint[a * SCT / 32];
            ammo[nextammo].xp = ammo[nextammo].x >> FIXP;
            ammo[nextammo].yp = ammo[nextammo].y >> FIXP;
            ammo[nextammo].oxp = ammo[nextammo].xp;
            ammo[nextammo].oyp = ammo[nextammo].xp;
            ammo[nextammo].hitdist = 8;
            ammo[nextammo].type = W_TOASTER;
            ammo[nextammo].xi = 4 * cost[a * SCT / 32];
            ammo[nextammo].yi = 4 * sint[a * SCT / 32];
            ammo[nextammo].damage = 5 * FIXS;
            ammo[nextammo].timeleft = 9999;
            nextammo = (nextammo + 1) % MAXAMMO;
        }
        break;
    case W_PULSLASER:
        ammo[nextammo].xi = cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 5 + plr[player].xi;
        ammo[nextammo].yi = sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 5 + plr[player].yi;
        ammo[nextammo].damage = 3 << FIXP;
        ammo[nextammo].timeleft = 9999;
        break;
    case W_ULTRATHR:
    case W_RETRO:
    case W_HYPERSPAC:
        ammo[nextammo].active = 0;
        ammo[nextammo].hitdist = 0;
        break;
    case W_DELBLAST:
        ammo[nextammo].x = plr[player].x + ((SHIPW / 2) << FIXP) -
                           cost[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 10 - FIXS;
        ammo[nextammo].y = plr[player].y + ((SHIPH / 2) << FIXP) -
                           sint[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 10 - FIXS;
        ammo[nextammo].xi = 0;
        ammo[nextammo].yi = 0;
        ammo[nextammo].damage = 25 << FIXP;
        ammo[nextammo].timeleft = 160;
        break;
    case W_SPILLER:
        a = (cnt / 4) & 31;
        ammo[nextammo].type = W_BLASTGUN;
        ammo[nextammo].x = plr[player].x + ((SHIPW / 2) << FIXP) + 12 * cost[a * SCT / 32];
        ammo[nextammo].y = plr[player].y + ((SHIPH / 2) << FIXP) + 12 * sint[a * SCT / 32];
        ammo[nextammo].xi = 4 * cost[a * SCT / 32];
        ammo[nextammo].yi = 4 * sint[a * SCT / 32];
        ammo[nextammo].damage = 4 << FIXP;
        ammo[nextammo].timeleft = 9999;
        break;
#ifdef REG
    case W_TUHNRUISK:
        /*r */ ammo[nextammo].type = W_TUHNU;
        ammo[nextammo].xi = cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].xi;
        ammo[nextammo].yi = sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].yi;
        ammo[nextammo].damage = FIXS / 2;
        ammo[nextammo].timeleft = 9999;
        break;
    case W_HYPERBLST:
        /*r */ ammo[nextammo].type = W_GASBLAST;
        ammo[nextammo].xi = cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].xi;
        ammo[nextammo].yi = sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].yi;
        ammo[nextammo].damage = 11 << FIXP;
        ammo[nextammo].timeleft = 9999;
        break;
    case W_TRACTBEAM:
        /*r */ ammo[nextammo].xi = cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].xi;
        ammo[nextammo].yi = sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 4 + plr[player].yi;
        ammo[nextammo].damage = FIXS / 2;
        ammo[nextammo].timeleft = 35;
        break;
    case W_TWINLASER:
        /*r */ ammo[nextammo].type = W_PULSLASER;
        ammo[nextammo].xi = cost[((((plr[player].dir + FIXS + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 5 + plr[player].xi;
        ammo[nextammo].yi = sint[((((plr[player].dir + FIXS + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 5 + plr[player].yi;
        ammo[nextammo].damage = 3 << FIXP;
        ammo[nextammo].timeleft = 9999;
        nextammo = (nextammo + 1) % MAXAMMO;
        ammo[nextammo].active = 1;
        ammo[nextammo].x = plr[player].x + ((SHIPW / 2) << FIXP) +
                           cost[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 10 + rand() % (FIXS * 2) - FIXS;
        ammo[nextammo].y = plr[player].y + ((SHIPH / 2) << FIXP) +
                           sint[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 10 + rand() % (FIXS * 2) - FIXS;
        ammo[nextammo].xp = ammo[nextammo].x >> FIXP;
        ammo[nextammo].yp = ammo[nextammo].y >> FIXP;
        ammo[nextammo].oxp = ammo[nextammo].xp;
        ammo[nextammo].oyp = ammo[nextammo].xp;
        ammo[nextammo].hitdist = 8;
        ammo[nextammo].type = W_PULSLASER;
        ammo[nextammo].xi = cost[((((plr[player].dir - FIXS + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 5 + plr[player].xi;
        ammo[nextammo].yi = sint[((((plr[player].dir - FIXS + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] * 5 + plr[player].yi;
        ammo[nextammo].damage = 3 << FIXP;
        ammo[nextammo].timeleft = 9999;
        break;
    case W_SUPERPYRO:
        /*r */ ammo[nextammo].x = plr[player].x + ((SHIPW / 2) << FIXP) -
                                  cost[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 12 - FIXS;
        ammo[nextammo].y = plr[player].y + ((SHIPH / 2) << FIXP) -
                           sint[((plr[player].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] * 12 - FIXS;
        ammo[nextammo].xi = -cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] + plr[player].xi / 2;
        ammo[nextammo].yi = -sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) + SCT * 3 / 4) % SCT] + plr[player].yi / 2;
        ammo[nextammo].damage = 10 << FIXP;
        ammo[nextammo].timeleft = 9999;
        break;
#endif
    }
    nextammo = (nextammo + 1) % MAXAMMO;
}

void newammoa(int nro)
{
    int a;
    if (nro == nextammo) {
        nextammo = (nextammo + 1) % MAXAMMO;
    }
    if (ammo[nro].active == 0) {
        return;
    }
    if (ammo[nextammo].active) {
        return;
    }
    ammo[nextammo].active = 1;
    ammo[nextammo].x = ammo[nro].x;
    ammo[nextammo].y = ammo[nro].y;
    ammo[nextammo].xp = ammo[nextammo].x >> FIXP;
    ammo[nextammo].yp = ammo[nextammo].y >> FIXP;
    ammo[nextammo].oxp = ammo[nextammo].xp;
    ammo[nextammo].oyp = ammo[nextammo].yp;
    ammo[nextammo].hitdist = 8;
    switch (ammo[nro].type) {
    case W_MORNSTAR:
        for (a = 0; a < 64; a++) {
            ammo[nextammo].active = 1;
            ammo[nextammo].x = ammo[nro].x + 2 * cost[a * SCT / 64];
            ammo[nextammo].y = ammo[nro].y + 2 * sint[a * SCT / 64];
            ammo[nextammo].xp = ammo[nextammo].x >> FIXP;
            ammo[nextammo].yp = ammo[nextammo].y >> FIXP;
            ammo[nextammo].oxp = ammo[nextammo].xp;
            ammo[nextammo].oyp = ammo[nextammo].yp;
            ammo[nextammo].hitdist = 8;
            ammo[nextammo].xi = 2 * cost[a * SCT / 64];
            ammo[nextammo].yi = 2 * sint[a * SCT / 64];
            ammo[nextammo].damage = FIXS * 4;
            ammo[nextammo].timeleft = 1500;
            ammo[nextammo].type = W_SLING;
            nextammo = (nextammo + 1) % MAXAMMO;
            if (nro == nextammo) {
                nextammo = (nextammo + 1) % MAXAMMO;
            }
            if (ammo[nextammo].active) {
                return;
            }
        }
        break;
    case W_LASH:
        ammo[nextammo].xi = 0;
        ammo[nextammo].yi = -FIXS;
        ammo[nextammo].damage = 3 << FIXP;
        ammo[nextammo].timeleft = 750;
        ammo[nextammo].type = W_DOT;
        break;
    case W_PYROTECH:
        ammo[nextammo].y = ammo[nro].y - (4 << FIXP);
        ammo[nextammo].xi = rand() % (FIXS * 2) - FIXS;
        ammo[nextammo].yi = rand() % (FIXS * 2) - FIXS * 4;
        ammo[nextammo].damage = 4 << FIXP;
        ammo[nextammo].timeleft = 1500;
        ammo[nextammo].type = W_MINIBLAST;
        break;
    case W_TOXICWAST:
        ammo[nextammo].x = ammo[nro].x + rand() % (FIXS * 4) - FIXS * 2;
        ammo[nextammo].y = ammo[nro].y - (4 << FIXP);
        ammo[nextammo].xi = rand() % (FIXS)-FIXS / 2;
        ammo[nextammo].yi = rand() % (FIXS / 4) - FIXS / 2;
        ammo[nextammo].damage = 4 << FIXP;
        ammo[nextammo].timeleft = 1500;
        ammo[nextammo].type = W_TOXICW2;
        break;
#ifdef REG
    case W_SUPERPYRO:
        ammo[nextammo].y = ammo[nro].y - (4 << FIXP);
        ammo[nextammo].xi = rand() % (FIXS * 2) - FIXS;
        ammo[nextammo].yi = rand() % (FIXS * 2) - FIXS * 4;
        ammo[nextammo].damage = 10 << FIXP;
        ammo[nextammo].timeleft = 1500;
        ammo[nextammo].type = W_BIGBLASTR;
        break;
#endif
    }
    nextammo = (nextammo + 1) % MAXAMMO;
}

void newpickammo()
{
    int typ = -1, prob, kp = -1;
    int spot = rand() % pickammospots;

    if (pickammo[spot].active) {
        return;
    }
    prob = rand() % totalprob;
    pickammo[spot].active = 1;
    while ((prob > kp) && (typ < BULLETBOXTS)) {
        typ++;
        kp += bulletbox[typ].prob;
    }
    if ((typ < 0) || (typ >= BULLETBOXTS)) {
        typ = 0;
    }
    //  pickammo[spot].active=1;
    //  typ=rand()%BULLETBOXTS;

    pickammo[spot].type = (Uint8)typ;
    pickammo[spot].timeleft = 4500;
    pickammo[spot].cnt = rand() % SCT;
    switch (typ) {
    case W_TUHNU:
    case W_PEAGUN:
    case W_SLING:
    case W_BIGPEAGUN:
    case W_BIGSLING:
    case W_BLASTGUN:
    case W_MINIBLAST:
    case W_BLASTER:
    case W_BIGBLASTR:
    case W_NEXTAMMO:
    case W_NOWEAPON:
        pickammo[spot].bullets = 0;
        break;
    case W_GASBLAST:
        pickammo[spot].bullets = rand() % 25 + 10;
        break;
    case W_MEGABLAST:
        pickammo[spot].bullets = rand() % 25 + 5;
        break;
    case W_MINE:
        pickammo[spot].bullets = rand() % 40 + 10;
        break;
    case W_HEATSEEK:
        pickammo[spot].bullets = rand() % 20 + 10;
        break;
    case W_MORNSTAR:
        pickammo[spot].bullets = rand() % 15 + 10;
        break;
    case W_SENSORJAM:
        pickammo[spot].bullets = rand() % 20 + 10;
        break;
    case W_PYROTECH:
        pickammo[spot].bullets = rand() % 11 + 1;
        break;
    case W_TOXICWAST:
        pickammo[spot].bullets = rand() % 14 + 4;
        break;
    case W_TOASTER:
        pickammo[spot].bullets = rand() % 1600 + 100;
        break;
    case W_REARSLING:
        pickammo[spot].bullets = rand() % 500 + 50;
        break;
    case W_RINGFIRE:
        pickammo[spot].bullets = rand() % 30 + 20;
        break;
    case W_CRAWLER:
        pickammo[spot].bullets = rand() % 40 + 20;
        break;
    case W_GRAVFLOW:
        pickammo[spot].bullets = rand() % 2000 + 500;
        break;
    case W_BOMB:
        pickammo[spot].bullets = rand() % 20 + 10;
        break;
    case W_LASH:
        pickammo[spot].bullets = rand() % 100 + 10;
        break;
    case W_ULTRATHR:
        pickammo[spot].bullets = rand() % 50 + 1;
        break;
    case W_RETRO:
        pickammo[spot].bullets = rand() % 15000 + 3000;
        break;
    case W_HYPERSPAC:
        pickammo[spot].bullets = (rand() % 15) * 2 + 2;
        break;
    case W_PLASMA:
        pickammo[spot].bullets = rand() % 20 + 2;
        break;
    case W_PULSLASER:
        pickammo[spot].bullets = rand() % 800 + 80;
        break;
    case W_DELBLAST:
        pickammo[spot].bullets = rand() % 20 + 2;
        break;
    case W_FLIPPER:
        pickammo[spot].bullets = rand() % 20 + 10;
        break;
    case W_SIGNAL:
        pickammo[spot].bullets = rand() % 20 + 10;
        break;
    case W_SPILLER:
        pickammo[spot].bullets = rand() % 200 + 50;
        break;
#ifdef REG
    case W_TUHNRUISK:
        pickammo[spot].bullets = rand() % 5000 + 1000;
        break;
    case W_HYPERBLST:
        pickammo[spot].bullets = rand() % 140 + 40;
        break;
    case W_TRACTBEAM:
        pickammo[spot].bullets = rand() % 2000 + 400;
        break;
    case W_TWINLASER:
        pickammo[spot].bullets = rand() % 500 + 50;
        break;
    case W_SUPERPYRO:
        pickammo[spot].bullets = rand() % 8 + 1;
        break;
#endif
    }
}

void newcrap(int xp, int yp, Uint8 type)
{
    crap[nextcrap].active = 1;
    crap[nextcrap].type = type;
    switch (type) {
    case 3: // water bubble
        crap[nextcrap].type = type;
        crap[nextcrap].x = xp << FIXP;
        crap[nextcrap].y = yp << FIXP;
        crap[nextcrap].xi = 0;
        crap[nextcrap].yi = -FIXS / 4;
        crap[nextcrap].xp = xp;
        crap[nextcrap].yp = yp;
        crap[nextcrap].timeleft = 9999;
        crap[nextcrap].c = 8;
        break;
    }
    nextcrap = (nextcrap + 1) % MAXCRAP;
}

void newplrcrap(Uint8 player, Uint8 type)
{
    int a;
    crap[nextcrap].active = 1;
    crap[nextcrap].type = type;
    switch (type) {
    case 0: // engine flame
        crap[nextcrap].x = plr[player].x + ((SHIPW / 2) << FIXP) + rand() % (FIXS)-FIXS / 2 +
                           cost[((plr[player].dir >> FIXP) * FRFIX + SCT * 1 / 4) % SCT] * 8 + rand() % (FIXS * 4) -
                           FIXS * 2;
        crap[nextcrap].y =
            plr[player].y + ((SHIPH / 2) << FIXP) + rand() % (FIXS)-FIXS / 2 +
            sint[((plr[player].dir >> FIXP) * FRFIX + SCT * 1 / 4) % SCT] * 8 + rand() % (FIXS * 4) -
            FIXS * 2;
        crap[nextcrap].xi =
            rand() % (FIXS)-FIXS / 2 -
            cost[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) +
                  SCT * 3 / 4) %
                 SCT];
        crap[nextcrap].yi =
            rand() % (FIXS)-FIXS / 2 -
            sint[((((plr[player].dir + rand() % (FIXS / 2) - FIXS / 4) * FRFIX) >> FIXP) +
                  SCT * 3 / 4) %
                 SCT];
        crap[nextcrap].xp = crap[nextcrap].x >> FIXP;
        crap[nextcrap].yp = crap[nextcrap].y >> FIXP;
        crap[nextcrap].timeleft = 13;
        /*if ((waterdetect(player)) && (!(rand() % 5)))
              { */
        /* bubbles in water */
        if ((waterdetect(plr[player].xp, plr[player].yp)) && (!(rand() % 5))) { /* bubbles in water */
            a = nextcrap;
            nextcrap = (nextcrap + 1) % MAXCRAP;
            newcrap(crap[a].xp, crap[a].yp, 3);
        }
        break;
    case 1: // flying water drops
        for (a = 0; a < 16; a++) {
            crap[nextcrap].active = 1;
            crap[nextcrap].type = type;
            crap[nextcrap].x = plr[player].x + ((SHIPW / 2) << FIXP);
            crap[nextcrap].y = plr[player].y + ((SHIPH / 2) << FIXP);
            crap[nextcrap].xi = cost[(SCT / 2 + a * (SCT / 2 / 16)) % SCT] * (rand() % 7 + 4) / 10;
            crap[nextcrap].yi = sint[(SCT / 2 + a * (SCT / 2 / 16)) % SCT] * (rand() % 7 + 4) / 10;
            crap[nextcrap].xp = crap[nextcrap].x >> FIXP;
            crap[nextcrap].yp = crap[nextcrap].y >> FIXP;
            crap[nextcrap].timeleft = 15;
            crap[nextcrap].c = (Uint8)(rand() % 2 + 7);
            nextcrap = (nextcrap + 1) % MAXCRAP;
        }
        break;
    case 4: // smoke when damaged
        crap[nextcrap].x = (plr[player].xp + SHIPW / 2 + rand() % 16 - 8) << FIXP;
        crap[nextcrap].y = (plr[player].yp + SHIPH / 2 + rand() % 8 - 8) << FIXP;
        crap[nextcrap].xi = rand() % (FIXS / 2) - FIXS / 4;
        crap[nextcrap].yi = rand() % (FIXS / 2) - FIXS;
        crap[nextcrap].xp = crap[nextcrap].x >> FIXP;
        crap[nextcrap].yp = crap[nextcrap].y >> FIXP;
        crap[nextcrap].timeleft = 15 + rand() % 20;
        crap[nextcrap].c = 128;
        a = nextcrap;
        nextcrap = (nextcrap + 1) % MAXCRAP;
        /*if ((waterdetect(player)) && (!(rand() % 5))) */
        if ((waterdetect(plr[player].xp, plr[player].yp)) && (!(rand() % 5))) {
            newcrap(crap[a].xp, crap[a].yp, 3);
        }
        break;
    }
    nextcrap = (nextcrap + 1) % MAXCRAP;
}

void newammocrap(int nro, char type)
{
    int a;
    crap[nextcrap].active = 1;
    crap[nextcrap].type = type;
    switch (type) {
    case 2: // flying ground pieces
        if ((ammo[nro].xp >= 1) && (ammo[nro].yp >= 1) &&
            (ammo[nro].xp < levw - 1) && (ammo[nro].yp < levh - 1)) {
            for (a = 0; a < 8; a++) {
                crap[nextcrap].active = 1;
                crap[nextcrap].type = type;
                crap[nextcrap].x = ammo[nro].xp << FIXP;
                crap[nextcrap].y = ammo[nro].yp << FIXP;
                crap[nextcrap].xi = rand() % (FIXS * 4) - FIXS * 2;
                crap[nextcrap].yi = rand() % (FIXS * 4) - FIXS * 2;
                crap[nextcrap].xp = crap[nextcrap].x >> FIXP;
                crap[nextcrap].yp = crap[nextcrap].y >> FIXP;
                crap[nextcrap].timeleft = 25;
                crap[nextcrap].c = *(level + crap[nextcrap].yp * levw + crap[nextcrap].xp);
                nextcrap = (nextcrap + 1) % MAXCRAP;
            }
        }
        break;
    case 3: // water bubble
        crap[nextcrap].type = type;
        crap[nextcrap].x = ammo[nro].x;
        crap[nextcrap].y = ammo[nro].y - (4 << FIXP);
        crap[nextcrap].xi = 0;
        crap[nextcrap].yi = -FIXS / 4;
        crap[nextcrap].xp = ammo[nro].xp;
        crap[nextcrap].yp = ammo[nro].yp;
        crap[nextcrap].timeleft = 9999;
        crap[nextcrap].c = 8;
        break;
    }
    nextcrap = (nextcrap + 1) % MAXCRAP;
}

void newboom(int xp, int yp, char type, char transp, char speed)
{
    boom[nextboom].active = 1;
    boom[nextboom].type = type;
    boom[nextboom].transp = transp;
    boom[nextboom].xp = xp;
    boom[nextboom].yp = yp;
    boom[nextboom].frm = 1;
    boom[nextboom].speed = speed;
    nextboom = (nextboom + 1) % MAXBOOMS;
}

void savescreenrix(char *fname)
{
    FILE *fp;
    Uint8 r, g, b;
    int a;
    Uint8 *tmp;
    Uint8 *tpic = NULL;

    fp = fopen(fname, "wb");
    tmp = (Uint8 *)malloc(307200);
    if (tmp == NULL) {
        jerror("SAVESCREENRIX/screen.malloc: No memory", 307200);
    }
    tpic = (Uint8 *)malloc(WINW * WINH + 4);
    if (tpic == NULL) {
        jerror("SAVESCREENRIX/tpic.malloc: No memory", WINW * WINH + 4);
    }
    util_loadpcx((char *)tpic, FILLERPICFILE, WINW * WINH);
    if (gamewindowcolorrand) {
        for (a = 0; a < WINW * WINH; a++) {
            *(tpic + a) -= 8;
        }
    }
    jcblockclip(0, 0, 320, 240, 0, 0, 639, 479, 640, tmp, tpic);
    jcblockclip(320, 0, 320, 240, 0, 0, 639, 479, 640, tmp, tpic);
    jcblockclip(0, 240, 320, 240, 0, 0, 639, 479, 640, tmp, tpic);
    jcblockclip(320, 240, 320, 240, 0, 0, 639, 479, 640, tmp, tpic);
    free(tpic);
    tpic = NULL;

    if (plr[0].active) {
        for (a = 0; a < WINH; a++) {
            memcpy(tmp + (a + 240) * 640 + 320, plr[0].scr + a * WINW, WINW);
        }
    }
    if (plr[1].active) {
        for (a = 0; a < WINH; a++) {
            memcpy(tmp + a * 640, plr[1].scr + a * WINW, WINW);
        }
    }
    if (plr[2].active) {
        for (a = 0; a < WINH; a++) {
            memcpy(tmp + a * 640 + 320, plr[2].scr + a * WINW, WINW);
        }
    }
    if (plr[3].active) {
        for (a = 0; a < WINH; a++) {
            memcpy(tmp + (a + 240) * 640, plr[3].scr + a * WINW, WINW);
        }
    }
    b = 0x52;
    fwrite(&b, 1, 1, fp);
    b = 0x49;
    fwrite(&b, 1, 1, fp);
    b = 0x58;
    fwrite(&b, 1, 1, fp);
    b = 0x33;
    fwrite(&b, 1, 1, fp);
    b = 0x80;
    fwrite(&b, 1, 1, fp);
    b = 0x02;
    fwrite(&b, 1, 1, fp);
    b = 0xE0;
    fwrite(&b, 1, 1, fp);
    b = 0x01;
    fwrite(&b, 1, 1, fp);
    b = 0xAF;
    fwrite(&b, 1, 1, fp);
    b = 0x00;
    fwrite(&b, 1, 1, fp);
    for (a = 0; a < 256; a++) {
        //                                              outp(0x3c7,a); r=inp(0x3c9); g=inp(0x3c9); b=inp(0x3c9);
        r = palette[a].r >> 2;
        g = palette[a].g >> 2;
        b = palette[a].b >> 2;
        fwrite(&r, 1, 1, fp);
        fwrite(&g, 1, 1, fp);
        fwrite(&b, 1, 1, fp);
    }
    fwrite(tmp, 307200, 1, fp);
    free(tmp);
    tmp = NULL;
    fclose(fp);
}
