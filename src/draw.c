/* draw.c
 * Copyright 1996,2001,2003 Jetro Lauha.
 *
 * $Id: draw.c,v 1.1 2003/04/15 18:15:49 tonic Exp $
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
#include "global.h"
#include "wport.h"
#include "spr.h"
#include "gamefunc.h"

unsigned int updatecounter = 0;

void level_parallax(Uint8 * dest, int winx, int winy)
{				/* or+and version */
    int a, b;
    Uint8 *destofs = dest;
    Uint8 *sourceofs = level + winy * levw + winx;
    Uint8 *backofs = backgr + (winy / 2) * backw + winx / 2;
    Uint8 *maskofs = mask + winy * levw + winx;
    for (b = 0; b < WINH; b++)
    {
	for (a = 0; a < WINW; a += 4)
        {
	    *(int *) (destofs) = *(int *) (sourceofs) | (*(int *) (backofs) & *(int *) (maskofs));
	    destofs += 4;
	    sourceofs += 4;
	    backofs += 4;
	    maskofs += 4;
	}
	sourceofs += levw - WINW;
	backofs += backw - WINW;
	maskofs += levw - WINW;
    }
}

void level_noparallax(Uint8 * dest, int winx, int winy)
{				/* only top layer */
    int b;
    Uint8 *destofs = dest;
    Uint8 *sourceofs = level + winy * levw + winx;
    for (b = 0; b < WINH; b++) {
        memcpy(destofs + b * WINW, sourceofs + b * levw, WINW);
    }
}

void parallax(Uint8 * dest, int winx, int winy)
{
    if (parallaxdraw) {
        level_parallax(dest, winx, winy);
    } else {
        level_noparallax(dest, winx, winy);
    }
}

void updatescreen()
{
    int p;
    Uint8 *out;
    int a, b, c;
    updatecounter++;
    drawnframes++;
    for (p = 0; p < MAXPLAYERS; p++)
    {
        switch (p)
        {
            case 0:
                if (plr[1].active)
                {
                    out = plr[1].scr;
                    if (plr[1].sensorjam > 0)
                    {
                        for (b = 0; b < WINH; b += 4) {
                            for (a = 0; a < WINW; a += 4) {
                                for (c = 0; c < 4; c++) {
                                    memset(out + (b + c) * WINW + a, *(out + (b * WINW + a)), 4);
                                }
                            }
                        }
                    }
                    if (plr[1].flip > 0)
                    {
                        for (b = 0; b < WINH; b++) {
                            memcpy(plr[1].tmp + b * WINW, plr[1].scr + (WINH - b - 1) * WINW, WINW);
                        }
                        out = plr[1].tmp;
                    }
                    else if (plr[1].signal > 0)
                    {
                        for (b = updatecounter & 3; b < WINH; b += 4) {
                            memcpy(plr[1].tmp + (b * WINW), plr[1].scr + (b * WINW), WINW);
                        }
                        out = plr[1].tmp;
                    }
                    jvcblock(0, 0, WINW, WINH, out);
                }
                break;
            case 1:
                if (plr[2].active)
                {
                    out = plr[2].scr;
                    if (plr[2].sensorjam > 0)
                    {
                        for (b = 0; b < WINH; b += 4) {
                            for (a = 0; a < WINW; a += 4) {
                                for (c = 0; c < 4; c++) {
                                    memset(out + (b + c) * WINW + a, *(out + (b * WINW + a)), 4);
                                }
                            }
                        }
                    }
                    if (plr[2].flip > 0)
                    {
                        for (b = 0; b < WINH; b++) {
                            memcpy(plr[2].tmp + b * WINW, plr[2].scr + (WINH - b - 1) * WINW, WINW);
                        }
                        out = plr[2].tmp;
                    }
                    else if (plr[2].signal > 0)
                    {
                        for (b = updatecounter & 3; b < WINH; b += 4) {
                            memcpy(plr[2].tmp + (b * WINW), plr[2].scr + (b * WINW), WINW);
                        }
                        out = plr[2].tmp;
                    }
                    jvcblock(320, 0, WINW, WINH, out);
                }
                break;
            case 2:
                if (plr[0].active)
                {
                    out = plr[0].scr;
                    if (plr[0].sensorjam > 0)
                    {
                        for (b = 0; b < WINH; b += 4) {
                            for (a = 0; a < WINW; a += 4) {
                                for (c = 0; c < 4; c++) {
                                    memset(out + (b + c) * WINW + a, *(out + (b * WINW + a)), 4);
                                }
                            }
                        }
                    }
                    if (plr[0].flip > 0)
                    {
                        for (b = 0; b < WINH; b++) {
                            memcpy(plr[0].tmp + b * WINW, plr[0].scr + (WINH - b - 1) * WINW, WINW);
                        }
                        out = plr[0].tmp;
                    }
                    else if (plr[0].signal > 0)
                    {
                        for (b = updatecounter & 3; b < WINH; b += 4) {
                            memcpy(plr[0].tmp + (b * WINW), plr[0].scr + (b * WINW), WINW);
                        }
                        out = plr[0].tmp;
                    }
                    if (players == 1) {
                        jvcblock(160, 120, WINW, WINH, out);
                    } else {
                        jvcblock(320, 240, WINW, WINH, out);
                    }
                }
                break;
            case 3:
                if (plr[3].active)
                {
                    out = plr[3].scr;
                    if (plr[3].sensorjam > 0)
                    {
                        for (b = 0; b < WINH; b += 4) {
                            for (a = 0; a < WINW; a += 4) {
                                for (c = 0; c < 4; c++) {
                                    memset(out + (b + c) * WINW + a, *(out + (b * WINW + a)), 4);
                                }
                            }
                        }
                    }
                    if (plr[3].flip > 0)
                    {
                        for (b = 0; b < WINH; b++) {
                            memcpy(plr[3].tmp + b * WINW, plr[3].scr + (WINH - b - 1) * WINW, WINW);
                        }
                        out = plr[3].tmp;
                    }
                    else if (plr[3].signal > 0)
                    {
                        for (b = updatecounter & 3; b < WINH; b += 4) {
                            memcpy(plr[3].tmp + (b * WINW), plr[3].scr + (b * WINW), WINW);
                        }
                        out = plr[3].tmp;
                    }
                    jvcblock(0, 240, WINW, WINH, out);
                }
                break;
        }
    }
    update();
}

void drawscreen()
{
    int a, p, x = 0, y = 0, ox, oy, o, b = 0, c = 0;

    /*  water waves */
    for (a = 0; a < levw; a++)
    {
        *(wave + a) =
            ((sint[((cnt << 2) + (a * 42) + SCT / 1000) % (SCT / 2)] +
              sint[(0xffffff - (cnt << 3) + (a * 52) + SCT / 3) % SCT] +
              sint[((cnt << 4) + (a * 71) + SCT * 2 / 3) % SCT]) >> (FIXP));
    }
    if (waterwaves > 0)
    {
        for (a = 0; a < waterwaves; a++)
        {
            for (b = 0; b < 3; b++)
            {
                *(level + (water[a].y + *(wave + water[a].x) + b) * levw + water[a].x) = 1;
                *(mask + (water[a].y + *(wave + water[a].x) + b) * levw +
                  water[a].x) =
                    wttrans[(water[a].y + *(wave + water[a].x) + b + wttable[water[a].x % WTTABLELEN]) % WTTRANSLEN];
            }
            for (b = 1; b < 4; b++)
            {
                *(level + (water[a].y + *(wave + water[a].x) - b) * levw + water[a].x) = 0;
                *(mask + (water[a].y + *(wave + water[a].x) - b) * levw + water[a].x) = (unsigned char) 255;
            }
        }
    }

    for (p = 0; p < MAXPLAYERS; p++) {
        if (plr[p].active)
        {
            /*  level */
            parallax(plr[p].scr, plr[p].wx, plr[p].wy);
            /*  crap */
            for (a = 0; a < MAXCRAP; a++) {
                if (crap[a].active)
                {
                    x = crap[a].xp - plr[p].wx;
                    y = crap[a].yp - plr[p].wy;
                    switch (crap[a].type)
                    {
                        case 0:
                        case 4:
                            o = y * WINW + x;
                            if ((x > 0) && (y > 0) && (x < WINW - 1)
                                && (y < WINH - 1))
                            {
                                switch (crap[a].type)
                                {
                                    case 0:
                                        b = 191 - crap[a].timeleft;
                                        break;
                                    case 4:
                                        b = rand() % 4 + 131;
                                        break;
                                }
                                if ((*(plr[p].scr + o) > 0)
                                    && (*(plr[p].scr + o) < 8))
                                {
                                    *(plr[p].scr + o - 1) = *(transp + (*(plr[p].scr + o - 1) << 8) + b);
                                    *(plr[p].scr + o + 1) = *(transp + (*(plr[p].scr + o + 1) << 8) + b);
                                    *(plr[p].scr + o + WINW) = *(transp + (*(plr[p].scr + o + WINW) << 8) + b);
                                    *(plr[p].scr + o - WINW) = *(transp + (*(plr[p].scr + o - WINW) << 8) + b);
                                }
                                else {
                                    if (rand() & 1) {
                                        *(plr[p].scr + o) = (Uint8) b;
                                    }
                                    if (rand() & 1) {
                                        *(plr[p].scr + o + 1) = (Uint8) b;
                                    }
                                    if (rand() & 1) {
                                        *(plr[p].scr + o + WINW) = (Uint8) b;
                                    }
                                    if (rand() & 1) {
                                        *(plr[p].scr + o + WINW + 1) = (Uint8) b;
                                    }
                                }
                            }
                            break;
                        case 1:
                        case 2:
                            if ((x >= 0) && (y >= 0) && (x < WINW)
                                && (y < WINH)) {
                                *(plr[p].scr + y * WINW + x) = crap[a].c;
                            }
                            break;
                        case 3:
                            if ((x > 0) && (y > 0) && (x < WINW - 1)
                                && (y < WINH - 1))
                            {
                                *(plr[p].scr + y * WINW + x + 1) = crap[a].c;
                                *(plr[p].scr + y * WINW + x - 1) = crap[a].c;
                                *(plr[p].scr + (y + 1) * WINW + x) = crap[a].c;
                                *(plr[p].scr + (y - 1) * WINW + x) = crap[a].c;
                            }
                            break;
                    }
                }
            }
            /*  ships (not own) */
            for (a = 0; a < (MAXPLAYERS + MAXBOTS); a++) {
                if ((plr[a].active) && (a != p))
                {
                    if (plr[a].energy > 0)
                    {
                        if (plr[a].hyperspac == 0)
                        {
                            if (plr[a].blink)
                            {
                                jcspriterepclip(plr[a].xp - plr[p].wx,
                                                plr[a].yp - plr[p].wy,
                                                SHIPW, SHIPH, 0, 0,
                                                WINW - 1, WINH - 1, WINW,
                                                (unsigned char) 255,
                                                plr[p].scr, plr[a].ship + (plr[a].dir >> FIXP) * SHIPSIZE);
                            }
                            else {
                                jcspriteclip(plr[a].xp - plr[p].wx,
                                             plr[a].yp - plr[p].wy, SHIPW,
                                             SHIPH, 0, 0, WINW - 1,
                                             WINH - 1, WINW, plr[p].scr,
                                             plr[a].ship + (plr[a].dir >> FIXP) * SHIPSIZE);
                            }
                        }
                        else {	/* hyperspace projector */
                            if (plr[a].blink)
                            {
                                jcspriterepclip(plr[a].xp - plr[p].wx,
                                                plr[a].yp - plr[p].wy,
                                                SHIPW, SHIPH, 0, 0,
                                                WINW - 1, WINH - 1, WINW,
                                                (Uint8) 255, plr[p].scr,
                                                plr[a].ship + (plr[a].dir >> FIXP) * SHIPSIZE);
                            }
                            else {
                                trsprite(plr[a].xp - plr[p].wx,
                                         plr[a].yp - plr[p].wy, SHIPW,
                                         SHIPH, 0, 0, WINW - 1, WINH - 1,
                                         WINW, plr[p].scr, plr[a].ship + (plr[a].dir >> FIXP) * SHIPSIZE);
                            }
                        }
                    }
                }
            }
            /*  own ship */
            if (plr[p].energy > 0)
            {
                if (plr[p].hyperspac == 0)
                {
                    if (plr[p].blink)
                    {
                        jcspriterepclip(plr[p].xp - plr[p].wx,
                                        plr[p].yp - plr[p].wy, SHIPW,
                                        SHIPH, 0, 0, WINW - 1, WINH - 1,
                                        WINW, (Uint8) 255, plr[p].scr,
                                        plr[p].ship + (plr[p].dir >> FIXP) * SHIPSIZE);
                    }
                    else {
                        jcspriteclip(plr[p].xp - plr[p].wx,
                                     plr[p].yp - plr[p].wy, SHIPW, SHIPH,
                                     0, 0, WINW - 1, WINH - 1, WINW,
                                     plr[p].scr, plr[p].ship + (plr[p].dir >> FIXP) * SHIPSIZE);
                    }
                }
                else {		/* hyperspace projector */
                    if (plr[p].blink)
                    {
                        jcspriterepclip(plr[p].xp - plr[p].wx,
                                        plr[p].yp - plr[p].wy, SHIPW,
                                        SHIPH, 0, 0, WINW - 1, WINH - 1,
                                        WINW, 255, plr[p].scr, plr[p].ship + (plr[p].dir >> FIXP) * SHIPSIZE);
                    }
                    else {
                        trsprite(plr[p].xp - plr[p].wx,
                                 plr[p].yp - plr[p].wy, SHIPW, SHIPH, 0, 0,
                                 WINW - 1, WINH - 1, WINW, plr[p].scr,
                                 plr[p].ship + (plr[p].dir >> FIXP) * SHIPSIZE);
                    }
                }
            }
            /*  ammos */
            for (a = 0; a < MAXAMMO; a++) {
                if (ammo[a].active)
                {
                    switch (ammo[a].type)
                    {
                        case W_TUHNU:
                        case W_PEAGUN:
                        case W_SLING:
                        case W_BIGPEAGUN:
                        case W_BIGSLING:
                        case W_BLASTGUN:
                        case W_MINIBLAST:
                        case W_BLASTER:
                        case W_BIGBLASTR:
                        case W_GASBLAST:
                        case W_MEGABLAST:
                        case W_HEATSEEK:
                        case W_MORNSTAR:
                        case W_SENSORJAM:
                        case W_TOASTER:
                        case W_REARSLING:
                        case W_BOMB:
                        case W_LASH:
                        case W_PYROTECH:
                        case W_TOXICWAST:
                        case W_GRAVFLOW:
                        case W_SIGNAL:
                        case W_FLIPPER:
                            x = ammo[a].xp - plr[p].wx - bullet[ammo[a].type].w / 2;
                            y = ammo[a].yp - plr[p].wy - bullet[ammo[a].type].h / 2;
                            jcspriteclip(x, y, bullet[ammo[a].type].w,
                                         bullet[ammo[a].type].h, 0, 0,
                                         WINW - 1, WINH - 1, WINW, plr[p].scr, bullet[ammo[a].type].pic);
                            break;
                        case W_TOXICW2:
                            x = ammo[a].xp - plr[p].wx - bullet[W_REARSLING].w / 2;
                            y = ammo[a].yp - plr[p].wy - bullet[W_REARSLING].h / 2;
                            jcspriteclip(x, y, bullet[W_REARSLING].w,
                                         bullet[W_REARSLING].h, 0, 0, WINW - 1,
                                         WINH - 1, WINW, plr[p].scr, bullet[W_REARSLING].pic);
                            break;
                        case W_MINE:
                            x = ammo[a].xp - plr[p].wx - bullet[ammo[a].type].w / 2;
                            y = ammo[a].yp - plr[p].wy - bullet[ammo[a].type].h / 2;
                            jcspriteclip(x, y, bullet[ammo[a].type].w,
                                         bullet[ammo[a].type].h, 0, 0,
                                         WINW - 1, WINH - 1, WINW, plr[p].scr,
                                         bullet[ammo[a].type].pic +
                                         ((ammo[a].timeleft >
                                           22400) ? 0 : (cnt >> 3) %
                                          bullet[ammo[a].type].frames) *
                                         bullet[ammo[a].type].w * bullet[ammo[a].type].h);
                            break;
                        case W_PLASMA:
                            x = ammo[a].xp - plr[p].wx;
                            y = ammo[a].yp - plr[p].wy;
                            sctrspritem(x, y, boomanim[3].w, boomanim[3].h,
                                        ammo[a].hitdist * 2,
                                        ammo[a].hitdist * 2, 0, 0, WINW - 1,
                                        WINH - 1, WINW, plr[p].scr, boomanim[3].pic + 10 * boomanim[3].siz);
                            break;
                        case W_CRAWLER:
                            x = ammo[a].xp - plr[p].wx - bullet[ammo[a].type].w / 2;
                            y = ammo[a].yp - plr[p].wy - bullet[ammo[a].type].h / 2;
                            jcspriteclip(x, y, bullet[ammo[a].type].w,
                                         bullet[ammo[a].type].h, 0, 0,
                                         WINW - 1, WINH - 1, WINW, plr[p].scr,
                                         bullet[ammo[a].type].pic +
                                         ((cnt >> 3) %
                                          bullet[ammo[a].type].frames) *
                                         bullet[ammo[a].type].w * bullet[ammo[a].type].h);
                            break;
                        case W_DOT:
                            x = ammo[a].xp - plr[p].wx;
                            y = ammo[a].yp - plr[p].wy;
                            if ((x >= 0) && (y >= 0) && (x < WINW) && (y < WINH)) {
                                *(plr[p].scr + y * WINW + x) = (Uint8) 255;
                            }
                            break;
                        case W_DELBLAST:
                            x = ammo[a].xp - plr[p].wx;
                            y = ammo[a].yp - plr[p].wy;
                            if ((x >= 0) && (y >= 0) && (x < WINW) && (y < WINH)) {
                                *(plr[p].scr + y * WINW + x) = 185;
                            }
                            break;
                        case W_PULSLASER:
                            x = ammo[a].xp - plr[p].wx;
                            y = ammo[a].yp - plr[p].wy;
                            ox = ammo[a].oxp - plr[p].wx;
                            oy = ammo[a].oyp - plr[p].wy;
                            if (((x >= 0) && (y >= 0) && (x < WINW)
                                 && (y < WINH)) || ((ox >= 0) && (oy >= 0)
                                                    && (ox < WINW)
                                                    && (oy < WINH)))
                            {
                                util_lineclip(x, y, ox, oy, 0, 0, WINW - 1, WINH - 1, 255, WINW, (char *) plr[p].scr);
                                util_lineclip(x + 1, y, ox, oy, 0, 0, WINW - 1,
                                              WINH - 1, 255, WINW, (char *) plr[p].scr);
                                util_lineclip(x, y + 1, ox, oy, 0, 0, WINW - 1,
                                              WINH - 1, 255, WINW, (char *) plr[p].scr);
                                util_lineclip(x + 1, y + 1, ox, oy, 0, 0, WINW - 1,
                                              WINH - 1, 255, WINW, (char *) plr[p].scr);
                            }
                            break;
#ifdef REG
                        case W_TRACTBEAM:
                            x = ammo[a].xp - plr[p].wx;
                            y = ammo[a].yp - plr[p].wy;
                            ox = ammo[a].oxp - plr[p].wx;
                            oy = ammo[a].oyp - plr[p].wy;
                            x = (x - (x + ox) / 2) * 2 + (x + ox) / 2;
                            y = (y - (y + oy) / 2) * 2 + (y + oy) / 2;
                            if (((x >= 0) && (y >= 0) && (x < WINW)
                                 && (y < WINH)) || ((ox >= 0) && (oy >= 0)
                                                    && (ox < WINW)
                                                    && (oy < WINH)))
                            {
                                util_lineclip(ox, oy, x, y, 0, 0, WINW - 1, WINH - 1, 139, WINW, (char *) plr[p].scr);
                                util_lineclip(ox + 1, oy, x, y, 0, 0, WINW - 1,
                                              WINH - 1, 139, WINW, (char *) plr[p].scr);
                                util_lineclip(ox, oy + 1, x, y, 0, 0, WINW - 1,
                                              WINH - 1, 139, WINW, (char *) plr[p].scr);
                                util_lineclip(ox + 1, oy + 1, x, y, 0, 0, WINW - 1,
                                              WINH - 1, 139, WINW, (char *) plr[p].scr);
                            }
                            break;
                        case W_SUPERPYRO:
                            x = ammo[a].xp - plr[p].wx - bullet[W_PYROTECH].w / 2;
                            y = ammo[a].yp - plr[p].wy - bullet[W_PYROTECH].h / 2;
                            jcspriteclip(x, y, bullet[W_PYROTECH].w,
                                         bullet[W_PYROTECH].h, 0, 0, WINW - 1,
                                         WINH - 1, WINW, plr[p].scr, bullet[W_PYROTECH].pic);
                            break;
#endif
                    }
                }
            }
            /*  booms */
            for (a = 0; a < MAXBOOMS; a++) {
                if (boom[a].active)
                {
                    if (boom[a].frm == 0)
                    {
                        if (boom[a].transp == 0)
                        {
                            exspriterep(boom[a].xp -
                                        boomanim[boom[a].type].w / 2,
                                        boom[a].yp -
                                        boomanim[boom[a].type].h / 2,
                                        boomanim[boom[a].type].w,
                                        boomanim[boom[a].type].h, 0, 0,
                                        levw - 1, levh - 1, levw, 0, level, boomanim[boom[a].type].pic);
                            exspriterep(boom[a].xp -
                                        boomanim[boom[a].type].w / 2,
                                        boom[a].yp -
                                        boomanim[boom[a].type].h / 2,
                                        boomanim[boom[a].type].w,
                                        boomanim[boom[a].type].h, 0, 0,
                                        levw - 1, levh - 1, levw, 0xff, mask, boomanim[boom[a].type].pic);
                        }
                        boom[a].active = 0;
                    }
                    else {
                        if (boom[a].transp == 0)
                        {
                            x = boom[a].xp - plr[p].wx - boomanim[boom[a].type].w / 2;
                            y = boom[a].yp - plr[p].wy - boomanim[boom[a].type].h / 2;
                            jcspriteclip(x, y, boomanim[boom[a].type].w,
                                         boomanim[boom[a].type].h, 0, 0,
                                         WINW - 1, WINH - 1, WINW,
                                         plr[p].scr,
                                         boomanim[boom[a].type].pic +
                                         boom[a].frm * boomanim[boom[a].type].siz);
                            exspriterep(boom[a].xp -
                                        boomanim[boom[a].type].w / 2,
                                        boom[a].yp -
                                        boomanim[boom[a].type].h / 2,
                                        boomanim[boom[a].type].w,
                                        boomanim[boom[a].type].h, 0, 0,
                                        levw - 1, levh - 1, levw, 0, level,
                                        boomanim[boom[a].type].pic +
                                        boom[a].frm * boomanim[boom[a].type].siz);
                            exspriterep(boom[a].xp -
                                        boomanim[boom[a].type].w / 2,
                                        boom[a].yp -
                                        boomanim[boom[a].type].h / 2,
                                        boomanim[boom[a].type].w,
                                        boomanim[boom[a].type].h, 0, 0,
                                        levw - 1, levh - 1, levw, 0xff,
                                        mask,
                                        boomanim[boom[a].type].pic +
                                        boom[a].frm * boomanim[boom[a].type].siz);
                        }
                        else {
                            x = boom[a].xp - plr[p].wx - boomanim[boom[a].type].w / 2;
                            y = boom[a].yp - plr[p].wy - boomanim[boom[a].type].h / 2;
                            trsprite(x, y, boomanim[boom[a].type].w,
                                     boomanim[boom[a].type].h, 0, 0,
                                     WINW - 1, WINH - 1, WINW, plr[p].scr,
                                     boomanim[boom[a].type].pic + boom[a].frm * boomanim[boom[a].type].siz);
                        }
                    }
                }
            }
            /*  pickammos */
            for (a = 0; a < pickammospots; a++) {
                if (pickammo[a].active)
                {
                    x = pickammo[a].xp - plr[p].wx - bulletbox[pickammo[a].type].w / 2;
                    y = pickammo[a].yp - plr[p].wy - bulletbox[pickammo[a].type].h / 2;
                    jcspriteclip(x, y, bulletbox[pickammo[a].type].w,
                                 bulletbox[pickammo[a].type].h, 0, 0,
                                 WINW - 1, WINH - 1, WINW, plr[p].scr, bulletbox[pickammo[a].type].pic);
                }
            }
            /*  energy */
            switch (p)
            {
                case 0:
                case 3:
                    y = 4;
                    break;
                case 1:
                case 2:
                    y = WINH - 8;
                    break;
            }
            b = y * WINW;
            for (a = 0; a < (plr[p].energy >> FIXP); a++)
            {
                switch (p)
                {
                    case 0:
                    case 2:
                        x = a * 3 + 4;
                        c = 140 - (x >> 6);
                        break;
                    case 1:
                    case 3:
                        x = WINW - 6 - a * 3;
                        c = 140 - ((a * 3 + 4) >> 6);
                        break;
                }
                o = b + x;
                *(plr[p].scr + o) = *(transp + (c << 8) + *(plr[p].scr + o));
                *(plr[p].scr + o + 1) = *(transp + (c << 8) + *(plr[p].scr + o + 1));
                *(plr[p].scr + o + WINW) = *(transp + (c << 8) + *(plr[p].scr + o + WINW));
                *(plr[p].scr + o + WINW + 1) = *(transp + (c << 8) + *(plr[p].scr + o + WINW + 1));
                *(plr[p].scr + o + WINW * 2) = *(transp + (c << 8) + *(plr[p].scr + o + WINW * 2));
                *(plr[p].scr + o + WINW * 2 + 1) = *(transp + (c << 8) + *(plr[p].scr + o + WINW * 2 + 1));
                *(plr[p].scr + o + 2) = *(transp + *(plr[p].scr + o + 2));
                *(plr[p].scr + o + WINW + 2) = *(transp + *(plr[p].scr + o + WINW + 2));
                *(plr[p].scr + o + WINW * 2 + 2) = *(transp + *(plr[p].scr + o + WINW * 2 + 2));
                *(plr[p].scr + o - WINW) = *(transp + *(plr[p].scr + o - WINW));
                *(plr[p].scr + o - WINW + 1) = *(transp + *(plr[p].scr + o - WINW + 1));
                *(plr[p].scr + o + WINW * 3) = *(transp + *(plr[p].scr + o + WINW * 3));
                *(plr[p].scr + o + WINW * 3 + 1) = *(transp + *(plr[p].scr + o + WINW * 3 + 1));
                switch (p)
                {
                    case 0:
                    case 2:
                        if (a == 0)
                        {
                            *(plr[p].scr + o - 1) = *(transp + *(plr[p].scr + o - 1));
                            *(plr[p].scr + o + WINW - 1) = *(transp + *(plr[p].scr + o + WINW - 1));
                            *(plr[p].scr + o + WINW * 2 - 1) = *(transp + *(plr[p].scr + o + WINW * 2 - 1));
                        }
                        break;
                    case 1:
                    case 3:
                        if (a == (plr[p].energy >> FIXP) - 1)
                        {
                            *(plr[p].scr + o - 1) = *(transp + *(plr[p].scr + o - 1));
                            *(plr[p].scr + o + WINW - 1) = *(transp + *(plr[p].scr + o + WINW - 1));
                            *(plr[p].scr + o + WINW * 2 - 1) = *(transp + *(plr[p].scr + o + WINW * 2 - 1));
                        }
                        break;
                }
            }
            /*  glowball (in base) */
            /*if (basedetect((Uint8) p) &&*/
            if (basedetect(plr[p].xp, plr[p].yp) &&
                plr[p].energy < (STARTENERGY << FIXP) && plr[p].hyperspac == (Uint8) 0 && plr[p].energy > 0)
            {
                x = plr[p].xp + SHIPW / 2 - plr[p].wx;
                y = plr[p].yp + SHIPH / 2 - plr[p].wy;
                b = (sint[(cnt * 15) % (SCT / 2) + SCT / 2] >> (FIXP - 5))
                    + 56;
                sctrspritem(x, y, othgfx[0].w, othgfx[0].h, b, b, 0, 0,
                            WINW - 1, WINH - 1, WINW, plr[p].scr, othgfx[0].pic);
                x = plr[p].xp + SHIPW / 2 - plr[p].wx;
                y = plr[p].yp + SHIPH / 2 - plr[p].wy;
                b = (sint[(cnt * 15) % (SCT / 2)] >> (FIXP - 3)) + 13;
                sctrspritem(x, y, othgfx[1].w, othgfx[1].h, b, b,
                            0, 0, WINW - 1, WINH - 1, WINW, plr[p].scr, othgfx[1].pic);
            }
            /*  weapon slots and texts (in base) */
            /*if ((basedetect((Uint8) p) == 1) && (plr[p].energy > 0)*/
            if ((basedetect(plr[p].xp, plr[p].yp) == 1) && (plr[p].energy > 0)
                && (plr[p].hyperspac == 0))
            {
                c = (WINW - (slots - 1) * 16 - 32) / 2;
                for (a = 0; a < slots; a++)
                {
                    if (plr[p].activeslot == a) {
                        jcscalespritem((c +=
                                        32) - 29 +
                                       bulletbox[plr[p].slots[a]].w,
                                       100 -
                                       bulletbox[plr[p].slots[a]].h / 2 +
                                       bulletbox[plr[p].slots[a]].h +
                                       *(wave + plr[p].wx + a),
                                       bulletbox[plr[p].slots[a]].w,
                                       bulletbox[plr[p].slots[a]].h,
                                       (bulletbox[plr[p].slots[a]].
                                        w << 1) -
                                       ((bulletbox[plr[p].slots[a]].w *
                                         ((sint[(cnt << 5) & (SCT - 1)] +
                                           FIXS) / 2)) >> (FIXP + 1)),
                                       (bulletbox[plr[p].slots[a]].
                                        h << 1) -
                                       ((bulletbox[plr[p].slots[a]].h *
                                         ((sint[(cnt << 5) & (SCT - 1)] +
                                           FIXS) / 2)) >> (FIXP + 1)), 0,
                                       0, WINW - 1, WINH - 1, WINW,
                                       plr[p].scr, bulletbox[plr[p].slots[a]].pic);
                    } else {
                        jcsprite((c +=
                                  16) - 16, 100 + *(wave + plr[p].wx + a),
                                 bulletbox[plr[p].slots[a]].w,
                                 bulletbox[plr[p].slots[a]].h, WINW,
                                 plr[p].scr, bulletbox[plr[p].slots[a]].pic);
                    }
                }
                kprintf(plr[p].scr, font, 5, 60, WINW, "normal weapon:");
                kprintf(plr[p].scr, font, 138, 60, WINW, wnames[plr[p].ammo1]);
                kprintf(plr[p].scr, font, 3, 80, WINW, "special weapon:");
                kprintf(plr[p].scr, font, 138, 80, WINW, wnames[plr[p].ammo2]);
                kprintf(plr[p].scr, font, 63, 120, WINW, "bullets:");
                kprintf(plr[p].scr, font, 138, 120, WINW, "%d", plr[p].bullets[plr[p].activeslot]);
            }

            /*  weapon boxes to corner of the player window */
            switch (p)
            {
                case 0:
                    x = WINW - bulletbox[plr[p].ammo1].w - 2;
                    y = 2;
                    break;
                case 1:
                    x = 2;
                    y = WINH - bulletbox[plr[p].ammo1].h - 2;
                    break;
                case 2:
                    x = WINW - bulletbox[plr[p].ammo1].w - 2;
                    y = WINH - bulletbox[plr[p].ammo1].h - 2;
                    break;
                case 3:
                    x = 2;
                    y = 2;
            }
            trsprite(x, y, bulletbox[plr[p].ammo1].w,
                     bulletbox[plr[p].ammo1].h, 0, 0, WINW - 1, WINH - 1,
                     WINW, plr[p].scr, bulletbox[plr[p].ammo1].pic);
            switch (p)
            {
                case 0:
                    x = WINW - bulletbox[plr[p].ammo2].w - 2;
                    y = bulletbox[plr[p].ammo1].h + 4;
                    break;
                case 1:
                    x = 2;
                    y = WINH - bulletbox[plr[p].ammo1].h - bulletbox[plr[p].ammo2].h - 4;
                    break;
                case 2:
                    x = WINW - bulletbox[plr[p].ammo2].w - 2;
                    y = WINH - bulletbox[plr[p].ammo1].h - bulletbox[plr[p].ammo2].h - 4;
                    break;
                case 3:
                    x = 2;
                    y = bulletbox[plr[p].ammo1].h + 4;
                    break;
            }
            trsprite(x, y, bulletbox[plr[p].ammo2].w,
                     bulletbox[plr[p].ammo2].h, 0, 0, WINW - 1, WINH - 1,
                     WINW, plr[p].scr, bulletbox[plr[p].ammo2].pic);

        }
    }

    for (a = 0; a < (MAXPLAYERS + MAXBOTS); a++)
    {
        if ((plr[a].active) && (plr[a].blink))
        {
            plr[a].blink = 0;
        }
    }
}
