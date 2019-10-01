/* spr.c
 * Copyright 1996,2001,2003 Jetro Lauha.
 *
 * $Id: spr.c,v 1.1 2003/04/15 18:15:49 tonic Exp $
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
#include <stdarg.h>
#include "global.h"
#include "wport.h"

#define EXMIN 1 /* exclude color range start */
#define EXMAX 7 /*        " "          stop */

void kprintf(Uint8 *dest, FONT fnt, int x, int y, int pgwid, char *format, ...)
{
    Uint8 msgbuf[256];
    int a, b;
    va_list arglist;
    va_start(arglist, format);
    vsprintf((char *)msgbuf, format, arglist);
    va_end(arglist);
    for (a = 0, b = x; (unsigned)a < strlen((char *)msgbuf); a++, b += fnt.w + 1) {
        jcsprite(b, y, fnt.w, fnt.h, pgwid, dest, fnt.o[msgbuf[a]]);
    }
}

void kprintfs(FONT fnt, int x, int y, char *format, ...)
{
    Uint8 msgbuf[256];
    int a, b;
    va_list arglist;
    va_start(arglist, format);
    vsprintf((char *)msgbuf, format, arglist);
    va_end(arglist);
    for (a = 0, b = x; (unsigned)a < strlen((char *)msgbuf); a++, b += fnt.w + 1) {
        jvcsprite(b, y, fnt.w, fnt.h, fnt.o[msgbuf[a]]);
    }
}

void sctrspritem(int mx, int my, int wid, int hei, int scwid, int schei,
                 int x1, int y1, int x2, int y2, int pgwid, Uint8 *dest, Uint8 *sour)
/* centered, scaled, clipped and transparent sprite routine */
{
    int xinc, yinc, tx, ty, u, v, v2, scro, souru, sourv, w, h, x, y, xx, yy, c, txwid = wid;
    Uint8 *scr;

    if ((scwid == 0) || (schei == 0)) {
        return;
    }
    xx = mx - (scwid >> 1);
    yy = my - (schei >> 1);
    if ((x1 > x2) || (y1 > y2) || (xx > x2) || (yy > y2) || (xx <= x1 - scwid) || (yy <= y1 - schei)) {
        return;
    }

    xinc = (wid << 16) / scwid;
    yinc = (hei << 16) / schei;

    souru = xinc / 2;
    sourv = yinc / 2; //subpixel
    x = xx;
    y = yy;
    w = scwid;
    h = schei;
    if (xx < x1) {
        x = x1;
        w -= x1 - xx;
        souru += (x1 - xx) * xinc;
    }
    if (yy < y1) {
        y = y1;
        h -= y1 - yy;
        sourv += (y1 - yy) * yinc;
    }
    if (x + w - 1 > x2) {
        w -= x + w - x2 - 1;
    }
    if (y + h - 1 > y2) {
        h -= y + h - y2 - 1;
    }

    v = sourv;
    scro = y * pgwid + x;
    for (ty = 0; ty < h; ty++) {
        scr = (Uint8 *)(dest + scro);
        u = souru;
        v2 = (v >> 16) * txwid;
        for (tx = 0; tx < w; tx++) {
            c = *(sour + (u >> 16) + v2);
            if (c) {
                *(Uint8 *)(scr) = *(transp + (c << 8) + *scr);
            }
            scr++;
            u += xinc;
        }
        scro += pgwid;
        v += yinc;
    }
}

void exspriterep(int xx, int yy, int width, int height, int x1, int y1,
                 int x2, int y2, int pgwid, Uint8 color, Uint8 *dest, Uint8 *sour)
/* clipped and color replaced sprite routine, doesn't overwrite colors 1-11 */
{
    int a, b, c, souro, wid, hei, x, y;

    if ((x1 > x2) || (y1 > y2) || (xx > x2) || (yy > y2) || (xx <= x1 - width) || (yy <= y1 - width)) {
        return;
    }
    souro = 0;
    x = xx;
    y = yy;
    wid = width;
    hei = height;
    if (xx < x1) {
        x = x1;
        wid = width - (x1 - xx);
        souro += (x1 - xx);
    }
    if (yy < y1) {
        y = y1;
        hei = height - (y1 - yy);
        souro += (y1 - yy) * width;
    }
    if (xx + wid - 1 > x2) {
        wid -= xx + wid - x2 - 1;
    }
    if (yy + hei - 1 > y2) {
        hei -= yy + hei - y2 - 1;
    }

    for (b = 0; b < hei; b++) {
        c = (y + b) * pgwid;
        for (a = 0; a < wid; a++) {
            if ((*(sour + souro + a) > 0) && ((*(dest + c + x + a) == EXMIN - 1) || (*(dest + c + x + a) > EXMAX))) {
                *(dest + c + x + a) = color;
            }
        }
        souro += width;
    }
}

void trsprite(int xx, int yy, int width, int height, int x1, int y1,
              int x2, int y2, int pgwid, Uint8 *dest, Uint8 *sour)
/* transparent and clipped sprite routine */
{
    int a, b, c, souro, wid, hei, x, y;

    if ((x1 > x2) || (y1 > y2) || (xx > x2) || (yy > y2) || (xx <= x1 - width) || (yy <= y1 - width)) {
        return;
    }
    souro = 0;
    x = xx;
    y = yy;
    wid = width;
    hei = height;
    if (xx < x1) {
        x = x1;
        wid = width - (x1 - xx);
        souro += (x1 - xx);
    }
    if (yy < y1) {
        y = y1;
        hei = height - (y1 - yy);
        souro += (y1 - yy) * width;
    }
    if (xx + wid - 1 > x2) {
        wid -= xx + wid - x2 - 1;
    }
    if (yy + hei - 1 > y2) {
        hei -= yy + hei - y2 - 1;
    }

    for (b = 0; b < hei; b++) {
        c = (y + b) * pgwid;
        for (a = 0; a < wid; a++) {
            if (*(sour + souro + a) > 0) {
                *(dest + c + x + a) = *(transp + (*(sour + souro + a) << 8) + *(dest + c + x + a));
            }
        }
        souro += width;
    }
}

void exline(int x1, int y1, int x2, int y2, int cx1, int cy1,
            int cx2, int cy2, Uint8 c, int pgwid, Uint8 *dest)
{
    int i, dx, dy, n, d, di1, di2, xi1, xi2, yi1, yi2;
    int x, y, o;
    dx = (((x2 - x1) < 0) ? -(x2 - x1) : (x2 - x1));
    dy = (((y2 - y1) < 0) ? -(y2 - y1) : (y2 - y1));

    if (dx >= dy) {
        n = dx + 1;
        d = (dy << 1) - dx;
        di1 = (dy << 1);
        di2 = ((dy - dx) << 1);
        xi1 = 1;
        xi2 = 1;
        yi1 = 0;
        yi2 = 1;
    } else {
        n = dy + 1;
        d = (dx << 1) - dy;
        di1 = (dx << 1);
        di2 = ((dx - dy) << 1);
        xi1 = 0;
        xi2 = 1;
        yi1 = 1;
        yi2 = 1;
    }
    if (x1 > x2) {
        xi1 = -xi1;
        xi2 = -xi2;
    }
    if (y1 > y2) {
        yi1 = -yi1;
        yi2 = -yi2;
    }
    x = x1;
    y = y1;
    for (i = 1; i <= n; i++) {
        o = y * pgwid + x;
        if ((x >= cx1) && (x < cx2) && (y >= cy1) && (y < cy2) &&
            ((*(dest + o) == EXMIN - 1) || (*(dest + o) > EXMAX))) {
            *(Uint8 *)(dest + o) = c;
        }
        if (d < 0) {
            d += di1;
            x += xi1;
            y += yi1;
        } else {
            d += di2;
            x += xi2;
            y += yi2;
        }
    }
}

void trline(int x1, int y1, int x2, int y2, Uint8 c, int pgwid, Uint8 *dest)
{
    int i, dx, dy, n, d, di1, di2, xi1, xi2, yi1, yi2;
    int x, y;
    dx = (((x2 - x1) < 0) ? -(x2 - x1) : (x2 - x1));
    dy = (((y2 - y1) < 0) ? -(y2 - y1) : (y2 - y1));

    if (dx >= dy) {
        n = dx + 1;
        d = (dy << 1) - dx;
        di1 = (dy << 1);
        di2 = ((dy - dx) << 1);
        xi1 = 1;
        xi2 = 1;
        yi1 = 0;
        yi2 = 1;
    } else {
        n = dy + 1;
        d = (dx << 1) - dy;
        di1 = (dx << 1);
        di2 = ((dx - dy) << 1);
        xi1 = 0;
        xi2 = 1;
        yi1 = 1;
        yi2 = 1;
    }
    if (x1 > x2) {
        xi1 = -xi1;
        xi2 = -xi2;
    }
    if (y1 > y2) {
        yi1 = -yi1;
        yi2 = -yi2;
    }
    x = x1;
    y = y1;
    for (i = 1; i <= n; i++) {
        *(Uint8 *)(dest + y * pgwid + x) = c;
        if (d < 0) {
            d += di1;
            x += xi1;
            y += yi1;
        } else {
            d += di2;
            x += xi2;
            y += yi2;
        }
    }
}
