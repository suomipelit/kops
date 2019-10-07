/* utilcore.c
 * Copyright 1996,1997,2001,2003 Jetro Lauha.
 *
 * $Id: utilcore.c,v 1.1 2003/04/15 18:15:57 tonic Exp $
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
#pragma warning(disable : 4115)
#include <windows.h>
#include <mmsystem.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

const int jrandseed1 = 0, jrandseed2 = 0;

static long randseed = 1L;

void crapsrand(Uint32 seed)
{
    randseed = (long)seed;
}

int craprand(void)
{
    randseed = randseed * 0x343fd + 0x269ec3;
    return (randseed >> 16) & 0x7fff;
}

Uint32 util_gettick()
{
#ifdef _WIN32
    return timeGetTime();
#else
    struct timeval tym;

    gettimeofday(&tym, NULL);
    return (tym.tv_sec * 100) + (tym.tv_usec * 100 / 1000000);
#endif
}

void util_line(int x1, int y1, int x2, int y2, Uint8 c, int pgwid, char *dest)
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
        *(char *)(dest + y * pgwid + x) = c;
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

void util_lineclip(int x1, int y1, int x2, int y2, int cx1, int cy1,
                   int cx2, int cy2, Uint8 c, int pgwid, char *dest)
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
        if ((x >= cx1) && (x < cx2) && (y >= cy1) && (y < cy2)) {
            *(char *)(dest + y * pgwid + x) = c;
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

#define UTIL_LOADPCX_GETBYT         \
    bl++;                           \
    if (bl > 9999) {                \
        bl = 0;                     \
        tmp = 10000;                \
        if (btsl < 10000)           \
            tmp = btsl;             \
        util_fread(buf, tmp, 1, f); \
        if (btsl > 9999)            \
            btsl -= 10000;          \
    }                               \
    getbyt = *(buf + bl)

void util_loadpcx(char *dest, char *filename, Uint32 bytes)
{
    Uint32 offs, bl, tmp;
    int btsl;
    Uint8 b1, b2;
    Uint32 a;
    UTIL_FILE *f;
    Uint8 *buf;
    Uint8 getbyt;

    if (util_finitialized == 0) {
        util_finit();
    }
    if (util_fileexists(filename)) {
        buf = (Uint8 *)malloc(10000);
        f = util_fopen(filename);
        offs = 0;
        util_fseek(f, 128, SEEK_SET);
        btsl = util_fsize(f) - 128;
        bl = 9999;
        do {
            UTIL_LOADPCX_GETBYT;
            b1 = getbyt;
            if ((b1 >> 6) == 3) {
                UTIL_LOADPCX_GETBYT;
                b2 = getbyt;
                for (a = 0; a < (Uint32)(b1 & 63); a++) {
                    *(dest + a + offs) = b2;
                }
                offs += (b1 & 63);
            } else {
                *(dest + offs) = b1;
                offs++;
            }
        } while (offs < bytes);
        util_fclose(f);
        free(buf);
    }
}

/* this ugly old pcx reader load the header straight with fread
 * (which is not the right way)
 */
#pragma pack(1)

typedef struct {
    char manufacturer, version, encoding, bitsperpixel;
    short int xmin, ymin, xmax, ymax; /* these are used */
    short int hdpi, vdpi;
    char colormap[48];
    char reserved, nplanes;
    short int bytesperline, paletteinfo;
    short int hscreensize, vscreensize;
    char filler[54];
} PCXHEADER;

#pragma pack()

void util_loadpcxpi(char *filename, PIC *pic)
{
    PCXHEADER header;
    Uint32 offs, bl, tmp, bytes = 0;
    int btsl;
    Uint8 a, b1, b2;
    UTIL_FILE *f;
    Uint8 *buf, *dest;
    Uint8 getbyt;

    if (util_finitialized == 0) {
        util_finit();
    }
    if (util_fileexists(filename)) {
        buf = (Uint8 *)malloc(10000);
        f = util_fopen(filename);
        offs = 0;
        util_fread(&header, sizeof(PCXHEADER), 1, f);
        pic->bitspp = 8;
        pic->bytespp = 1;
        pic->w = header.xmax - header.xmin + 1;
        pic->h = header.ymax - header.ymin + 1;
        bytes = (pic->w) * (pic->h);
        pic->pic = (Uint8 *)malloc(bytes);
        dest = (Uint8 *)pic->pic;
        btsl = util_fsize(f) - 128;
        bl = 9999;
        while (offs < bytes) {
            UTIL_LOADPCX_GETBYT;
            b1 = getbyt;
            if ((b1 >> 6) == 3) {
                UTIL_LOADPCX_GETBYT;
                b2 = getbyt;
                for (a = 0; a < (b1 & 63); a++) {
                    *(dest + a + offs) = b2;
                }
                offs += (b1 & 63);
            } else {
                *(dest + offs) = b1;
                offs++;
            }
        }
        pic->pal = (Uint8 *)malloc(768);
        util_fseek(f, -768, SEEK_END);
        util_fread(pic->pal, 768, 1, f);
        for (offs = 0; offs < 768; offs++) {
            *(pic->pal + offs) >>= 2;
        }
        util_fclose(f);
        free(buf);
    }
}

void util_freepi(PIC *pic)
{
    free(pic->pic);
    pic->pic = NULL;
    free(pic->pal);
    pic->pal = NULL;
}

void util_xorrand(char *data, int jrandseed, int datalen)
{
    int a, saverand;
    saverand = rand();
    crapsrand(jrandseed);
    for (a = 0; a < datalen; a++) {
        *(data + a) ^= craprand() & 255;
    }
    srand(saverand);
}

Uint32 util_filesize(FILE *fp)
{
    int savepos, sizeoffile;
    savepos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    sizeoffile = ftell(fp);
    fseek(fp, savepos, SEEK_SET);
    return (sizeoffile);
}

char util_fileexists(char *filename)
{
    UTIL_FILE *fp;
    if (util_finitialized == 0) {
        util_finit();
    }
    fp = util_fopen(filename);
    if (fp != NULL) {
        util_fclose(fp);
        return (1);
    }
    return (0);
}
