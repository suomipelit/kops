/* wport.c
 * Copyright 1996,1997,2001,2003 Jetro Lauha.
 *
 * $Id: wport.c,v 1.1 2003/04/15 18:15:49 tonic Exp $
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

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "global.h"
#include "wport.h"

volatile short int cstart[768], cdest[768], ctemp[768];
volatile short int realfadecount;

SDL_Color palette[256];
SDL_Surface *screen = NULL;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

int waskey(int key)
{
    // SP-TODO
    return 0;
}

int key(int key)
{
    // SP-TODO
    return 0;
}

void clearkey(int key)
{
    // SP-TODO
}

void update()
{
    SDL_Event ev;

    // SP-TODO
    while (SDL_PollEvent(&ev))
    {
	switch (ev.type)
        {
            case SDL_KEYDOWN:
                //key(ev.key.keysym.sym) = 1;
                //waskey(ev.key.keysym.sym) = 1;
                break;
            case SDL_KEYUP:
                //key(ev.key.keysym.sym) = 0;
                break;
            case SDL_QUIT:
                exit(0);
                break;
	}
    }
    SDL_UpdateTexture(texture, NULL, screen->pixels, X_RESOLUTION * sizeof(Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

Uint8 fileexists(char *filename)
{
    UTIL_FILE *fp;
    if (util_finitialized == 0)
	util_finit();
    fp = util_fopen(filename);
    if (fp != NULL)
    {
	util_fclose(fp);
	return (1);
    }
    return (0);
}

void jcgetblock(int x, int y, int wid, int hei, int pgwid, Uint8 * sour, Uint8 * dest)
{
    register int b, od, os;
    od = 0;
    os = y * pgwid + x;
    for (b = 0; b < hei; b++)
    {
	memcpy(dest + od, sour + os, wid);
	od += wid;
	os += pgwid;
    }
}

void jcblockclip(int xx, int yy, int width, int height, int x1, int y1,
		 int x2, int y2, int pgwid, Uint8 * dest, Uint8 * sour)
{
    register int b, desto, souro, wid, hei, x, y;

    if ((x1 > x2) || (y1 > y2) || (xx > x2) || (yy > y2) || (xx <= x1 - width) || (yy <= y1 - height))
	return;
    souro = 0;
    x = xx;
    y = yy;
    wid = width;
    hei = height;
    if (xx < x1)
    {
	x = x1;
	wid = width - (x1 - xx);
	souro += (x1 - xx);
    }
    if (yy < y1)
    {
	y = y1;
	hei = height - (y1 - yy);
	souro += (y1 - yy) * width;
    }
    if (x + wid - 1 > x2)
    {
	wid -= x + wid - x2 - 1;
    }
    if (y + hei - 1 > y2)
    {
	hei -= y + hei - y2 - 1;
    }
    desto = y * pgwid + x;

    for (b = 0; b < hei; b++)
    {
	memcpy(dest + desto, sour + souro, wid);
	desto += pgwid;
	souro += width;
    }
}

void jcblock(int x, int y, int wid, int hei, int pgwid, Uint8 * dest, Uint8 * sour)
{
    register int b, od, os;
    od = y * pgwid + x;
    os = 0;
    for (b = 0; b < hei; b++)
    {
	memcpy(dest + od, sour + os, wid);
	od += pgwid;
	os += wid;
    }
}

void jvcblock(int x, int y, int wid, int hei, Uint8 * sour)
{
    if (SDL_MUSTLOCK(screen) && SDL_LockSurface(screen) < 0)
	return;
    jcblock(x, y, wid, hei, screen->pitch, (Uint8 *) screen->pixels, sour);
    if (SDL_MUSTLOCK(screen))
	SDL_UnlockSurface(screen);
}

void jcspriterepclip(int xx, int yy, int width, int height, int x1, int y1,
		     int x2, int y2, int pgwid, Uint8 color, Uint8 * dest, Uint8 * sour)
{
    int a, b, c, souro, wid, hei, x, y;

    if ((x1 > x2) || (y1 > y2) || (xx > x2) || (yy > y2) || (xx <= x1 - width) || (yy <= y1 - height))
	return;
    souro = 0;
    x = xx;
    y = yy;
    wid = width;
    hei = height;
    if (xx < x1)
    {
	x = x1;
	wid = width - (x1 - xx);
	souro += (x1 - xx);
    }
    if (yy < y1)
    {
	y = y1;
	hei = height - (y1 - yy);
	souro += (y1 - yy) * width;
    }
    if (x + wid - 1 > x2)
    {
	wid -= x + wid - x2 - 1;
    }
    if (y + hei - 1 > y2)
    {
	hei -= y + hei - y2 - 1;
    }

    for (b = 0; b < hei; b++)
    {
	c = (y + b) * pgwid;
	for (a = 0; a < wid; a++)
	    if (*(sour + souro + a) > 0)
		*(dest + c + x + a) = color;
	souro += width;
    }
}

void jcspriteclip(int xx, int yy, int width, int height, int x1, int y1,
		  int x2, int y2, int pgwid, Uint8 * dest, Uint8 * sour)
{
    register int a, b, c, souro, wid, hei, x, y;

    if ((x1 > x2) || (y1 > y2) || (xx > x2) || (yy > y2) || (xx <= x1 - width) || (yy <= y1 - height))
	return;
    souro = 0;
    x = xx;
    y = yy;
    wid = width;
    hei = height;
    if (xx < x1)
    {
	x = x1;
	wid = width - (x1 - xx);
	souro += (x1 - xx);
    }
    if (yy < y1)
    {
	y = y1;
	hei = height - (y1 - yy);
	souro += (y1 - yy) * width;
    }
    if (x + wid - 1 > x2)
    {
	wid -= x + wid - x2 - 1;
    }
    if (y + hei - 1 > y2)
    {
	hei -= y + hei - y2 - 1;
    }
    c = y * pgwid + x;

    for (b = 0; b < hei; b++)
    {
	for (a = 0; a < wid; a++)
        {
	    if (*(sour + souro) > 0)
		*(dest + c) = *(sour + souro);
	    c++;
	    souro++;
	}
	c += pgwid - wid;
	souro += width - wid;
    }
}

void jcscalespritem(int mx, int my, int wid, int hei, int scwid, int schei,
		    int x1, int y1, int x2, int y2, int pgwid, Uint8 * dest, Uint8 * sour)
{
    int xinc, yinc, tx, ty, u, v, v2, scro, souru, sourv, w, h, x, y, xx, yy, c, txwid = wid;
    Uint8 *scr;

    if ((scwid == 0) || (schei == 0))
	return;
    xx = mx - (scwid >> 1);
    yy = my - (schei >> 1);
    if ((x1 > x2) || (y1 > y2) || (xx > x2) || (yy > y2) || (xx <= x1 - scwid) || (yy <= y1 - schei))
	return;

    xinc = (wid << 16) / scwid;
    yinc = (hei << 16) / schei;

    souru = xinc / 2;
    sourv = yinc / 2;		/* subpixel */
    x = xx;
    y = yy;
    w = scwid;
    h = schei;
    if (xx < x1)
    {
	x = x1;
	w -= x1 - xx;
	souru += (x1 - xx) * xinc;
    }
    if (yy < y1)
    {
	y = y1;
	h -= y1 - yy;
	sourv += (y1 - yy) * yinc;
    }
    if (x + w - 1 > x2)
    {
	w -= x + w - x2 - 1;
    }
    if (y + h - 1 > y2)
    {
	h -= y + h - y2 - 1;
    }

    v = sourv;
    scro = y * pgwid + x;
    for (ty = 0; ty < h; ty++)
    {
	scr = (Uint8 *) (dest + scro);
	u = souru;
	v2 = (v >> 16) * txwid;
	for (tx = 0; tx < w; tx++)
        {
	    if ((c = *(sour + (u >> 16) + v2)))
		*((Uint8 *) (scr++)) = (Uint8) c;
	    else
		scr++;
	    u += xinc;
	}
	scro += pgwid;
	v += yinc;
    }
}

void jcsprite(int x, int y, int wid, int hei, int pgwid, Uint8 * dest, Uint8 * sour)
{
    register int a, b, c, souro;
    souro = 0;
    c = y * pgwid + x;
    for (b = 0; b < hei; b++)
    {
	for (a = 0; a < wid; a++)
        {
	    if (*(sour + souro) > 0)
		*(dest + c) = *(sour + souro);
	    souro++;
	    c++;
	}
	c += pgwid - wid;
    }
}

void jsavestart()
{
    int a;
    for (a = 0; a < 256; ++a)
    {
	cstart[a * 3] = palette[a].r >> 2;
	cstart[a * 3 + 1] = palette[a].g >> 2;
	cstart[a * 3 + 2] = palette[a].b >> 2;
    }
}

void jcleardest()
{
    unsigned short int a;
    for (a = 0; a < 768; a++)
	cdest[a] = 0;
}

void jinitreal()
{
    unsigned short int a;
    for (a = 0; a < 768; a++)
	ctemp[a] = (short) (cstart[a] << 7);
    realfadecount = 0;
}

void jsavedest()
{
    int a;
    for (a = 0; a < 256; ++a)
    {
	cdest[a * 3] = palette[a].r >> 2;
	cdest[a * 3 + 1] = palette[a].g >> 2;
	cdest[a * 3 + 2] = palette[a].b >> 2;
    }
}

void jrealfade(Uint8 from, Uint8 tocl, Uint8 frames)
{
    int a, b;
    for (a = 0; a < 768; ++a)
	ctemp[a] = cstart[a] << 7;
    for (b = 0; b < frames; ++b)
    {
	for (a = from * 3; a < tocl * 3 + 3; ++a)
	    ctemp[a] += ((cdest[a] << 7) - (cstart[a] << 7)) / frames;
	for (a = from; a <= tocl; ++a)
        {
	    palette[a].r = ctemp[a * 3] >> 5;
	    palette[a].g = ctemp[a * 3 + 1] >> 5;
	    palette[a].b = ctemp[a * 3 + 2] >> 5;
	}
	//SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, palette, from, tocl - from + 1); SP-TODO
	update();
    }
    for (a = from; a <= tocl; ++a)
    {
	palette[a].r = cdest[a * 3] << 2;
	palette[a].g = cdest[a * 3 + 1] << 2;
	palette[a].b = cdest[a * 3 + 2] << 2;
    }
    //SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, palette, from, tocl - from + 1); SP-TODO
    update();
}

void jrealfade1(Uint8 from, Uint8 tocl, Uint8 frames)
{
    int a;
    ++realfadecount;
    if (realfadecount <= frames)
    {
	for (a = from * 3; a < tocl * 3 + 3; ++a)
	    ctemp[a] += ((cdest[a] << 7) - (cstart[a] << 7)) / frames;
	for (a = from; a <= tocl; ++a)
        {
	    palette[a].r = ctemp[a * 3] >> 5;
	    palette[a].g = ctemp[a * 3 + 1] >> 5;
	    palette[a].b = ctemp[a * 3 + 2] >> 5;
	}
	//SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, palette, from, tocl - from + 1); SP-TODO
    }
    else {
	for (a = from; a <= tocl; ++a)
        {
	    palette[a].r = cdest[a * 3] << 2;
	    palette[a].g = cdest[a * 3 + 1] << 2;
	    palette[a].b = cdest[a * 3 + 2] << 2;
	}
	//SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, palette, from, tocl - from + 1); SP-TODO
    }
}

void jvrc()
{
    update();
}

void jvcfillbox(int x, int y, int x2, int y2, Uint8 color)
{
    int b;
    if (SDL_MUSTLOCK(screen) && SDL_LockSurface(screen) < 0)
	return;
    for (b = y; b <= y2; ++b)
	memset((Uint8 *) screen->pixels + b * screen->pitch + x, color, x2 - x + 1);
    if (SDL_MUSTLOCK(screen))
	SDL_UnlockSurface(screen);
}

void jerror(char *description, int errorcode)
{
    char sdf[1024];

    sprintf(sdf, "%s (%d)", description, errorcode);
#ifdef _WIN32
    // Use fancy GUI error box for windows version
    MessageBox(NULL, sdf, "KOPS error - Sorry!", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
#else
    // Otherwise be lame and print to stdout
    printf("\n\nKOPS error - Sorry!\n%s\n\n", sdf);
    fflush(stdout);
#endif
    exit(errorcode);
}

void jloadpcxpalpartdest(char *filename, Uint8 from, Uint8 tocl)
{
    UTIL_FILE *f;
    unsigned short int a;
    Uint8 *buf;
    if (util_finitialized == 0)
	util_finit();
    f = util_fopen(filename);
    if (f != NULL)
    {
	buf = (Uint8 *) malloc(768);
	util_fseek(f, -768, SEEK_END);
	util_fread(buf, 768, 1, f);
	for (a = from * 3; a < tocl * 3 + 3; a++)
	    cdest[a] = (short) (*(buf + a) >> 2);
	free(buf);
	util_fclose(f);
    }
}

void jloadpcxpalpart(char *filename, Uint8 from, Uint8 tocl)
{
    UTIL_FILE *f;
    unsigned short int a;
    Uint8 *buf;
    if (util_finitialized == 0)
	util_finit();
    f = util_fopen(filename);
    if (f != NULL)
    {
	buf = (Uint8 *) malloc(768);
	util_fseek(f, -768, SEEK_END);
	util_fread(buf, 768, 1, f);
	for (a = from; a <= tocl; ++a)
        {
	    palette[a].r = *(buf + a * 3);
	    palette[a].g = *(buf + a * 3 + 1);
	    palette[a].b = *(buf + a * 3 + 2);
	}
	//SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, palette, from, tocl - from + 1); SP-TODO
	free(buf);
	util_fclose(f);
    }
}

void jclearpal()
{
    int a;
    for (a = 0; a < 256; ++a)
	palette[a].r = palette[a].g = palette[a].b = 0;
    //SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, palette, 0, 256); SP-TODO

}

void jclearstart()
{
    unsigned short int a;
    for (a = 0; a < 768; a++)
	cstart[a] = 0;
}

void jpal(int color, int r, int g, int b)
{
    palette[color].r = r << 2;
    palette[color].g = g << 2;
    palette[color].b = b << 2;
    //SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, palette, color, 1); SP-TODO

}

void jvcsprite(int x, int y, int wid, int hei, Uint8 * sour)
{
    if (SDL_MUSTLOCK(screen) && SDL_LockSurface(screen) < 0)
	return;
    jcsprite(x, y, wid, hei, screen->pitch, (Uint8 *) screen->pixels, sour);
    if (SDL_MUSTLOCK(screen))
	SDL_UnlockSurface(screen);
}

void jvdump(int startofs, int pixels, Uint8 * sour)
{
    if (SDL_MUSTLOCK(screen) && SDL_LockSurface(screen) < 0)
	return;
    memcpy((Uint8 *) screen->pixels + startofs, sour, pixels);
    if (SDL_MUSTLOCK(screen))
	SDL_UnlockSurface(screen);

}

void jvcblockclip(int xx, int yy, int width, int height, int x1, int y1, int x2, int y2, Uint8 * sour)
{
    if (SDL_MUSTLOCK(screen) && SDL_LockSurface(screen) < 0)
	return;
    jcblockclip(xx, yy, width, height, x1, y1, x2, y2, screen->pitch, (Uint8 *) screen->pixels, sour);
    if (SDL_MUSTLOCK(screen))
	SDL_UnlockSurface(screen);
}

void jvline(int x1, int y1, int x2, int y2, Uint8 c)
{
    if (SDL_MUSTLOCK(screen) && SDL_LockSurface(screen) < 0)
	return;
    util_line(x1, y1, x2, y2, c, screen->pitch, (char *) screen->pixels);
    if (SDL_MUSTLOCK(screen))
	SDL_UnlockSurface(screen);
}

void jcscalesprite(int xx, int yy, int wid, int hei, int scwid, int schei,
		   int x1, int y1, int x2, int y2, int pgwid, Uint8 * dest, Uint8 * sour)
{
    int xinc, yinc, tx, ty, u, v, v2, scro, souru, sourv, w, h, x, y, c, txwid = wid;
    Uint8 *scr;

    if ((scwid == 0) || (schei == 0))
	return;
    if ((x1 > x2) || (y1 > y2) || (xx > x2) || (yy > y2) || (xx <= x1 - scwid) || (yy <= y1 - schei))
	return;

    xinc = (wid << 16) / scwid;
    yinc = (hei << 16) / schei;

    souru = xinc / 2;
    sourv = yinc / 2;		/* subpixel */
    x = xx;
    y = yy;
    w = scwid;
    h = schei;
    if (xx < x1)
    {
	x = x1;
	w -= x1 - xx;
	souru += (x1 - xx) * xinc;
    }
    if (yy < y1)
    {
	y = y1;
	h -= y1 - yy;
	sourv += (y1 - yy) * yinc;
    }
    if (x + w - 1 > x2)
    {
	w -= x + w - x2 - 1;
    }
    if (y + h - 1 > y2)
    {
	h -= y + h - y2 - 1;
    }

    v = sourv;
    scro = y * pgwid + x;
    for (ty = 0; ty < h; ty++)
    {
	scr = (Uint8 *) (dest + scro);
	u = souru;
	v2 = (v >> 16) * txwid;
	for (tx = 0; tx < w; tx++)
        {
	    if ((c = *(sour + (u >> 16) + v2)))
		*((Uint8 *) (scr++)) = (Uint8) c;
	    else
		scr++;
	    u += xinc;
	}
	scro += pgwid;
	v += yinc;
    }
}
