/* load.c
 * Copyright 1996,2001,2003 Jetro Lauha.
 *
 * $Id: load.c,v 1.1 2003/04/15 18:15:49 tonic Exp $
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
#ifdef _WIN32
#define ZLIB_DLL
#endif
#include "zlib.h"

void saveconfig()
{
    int b, c;
    FILE *fp;
    fp = fopen(KOPSCFGFILE, "wb");
    fwrite(&versmaj, 4, 1, fp);
    fwrite(&versmin, 4, 1, fp);
    c = 0;
    for (b = 0; b < MAXPLAYERS; b++)
    {
	fwrite(&c, 1, 1, fp);
	fwrite(&plr[b].k_up, 4, 1, fp);
	fwrite(&plr[b].k_left, 4, 1, fp);
	fwrite(&plr[b].k_right, 4, 1, fp);
	fwrite(&plr[b].k_fire1, 4, 1, fp);
	fwrite(&plr[b].k_fire2, 4, 1, fp);
    }
    fwrite(&players, 1, 1, fp);
    fwrite(&bots, 1, 1, fp);
    fwrite(&parallaxdraw, 1, 1, fp);
    fwrite(&slots, 1, 1, fp);
    fwrite(&s_sdevice, 1, 1, fp);
    fwrite(&s_samprate, 4, 1, fp);
//      fwrite(&s_interp,1,1,fp);
    fwrite(&s_shortchn, 1, 1, fp);
    fwrite(&s_longchn, 1, 1, fp);
    fwrite(&s_musvol, 1, 1, fp);
    fwrite(&s_effvol, 1, 1, fp);
    fwrite(&gravvalue, 1, 1, fp);
    fclose(fp);
}

void newconfig()
{
    plr[0].k_up     = K_UP;
    plr[0].k_left   = K_LEFT;
    plr[0].k_right  = K_RIGHT;
    plr[0].k_fire1  = K_RIGHTSHIFT;
    plr[0].k_fire2  = K_DOWN;
    plr[1].k_up     = K_W;
    plr[1].k_left   = K_A;
    plr[1].k_right  = K_D;
    plr[1].k_fire1  = K_LEFTSHIFT;
    plr[1].k_fire2  = K_S;
    plr[2].k_up     = K_Y;
    plr[2].k_left   = K_G;
    plr[2].k_right  = K_J;
    plr[2].k_fire1  = K_F;
    plr[2].k_fire2  = K_H;
    //  plr[3].k_up=26; plr[3].k_left=39; plr[3].k_right=43;
    //  plr[3].k_fire1=K_L; plr[3].k_fire2=40;
    plr[3].k_up     = K_L;
    plr[3].k_left   = SDLK_COMMA;
    plr[3].k_right  = SDLK_SLASH;
    plr[3].k_fire1  = K_M;
    plr[3].k_fire2  = SDLK_PERIOD;

    parallaxdraw = 1;
    players = 2;
    bots    = 0;
    slots = 16;
    s_sdevice = 1;
    s_samprate = 44100;
    s_interp = 0;
    s_shortchn = 8;
    s_longchn = 8;
    s_musvol = 24;
    s_effvol = 64;
    gravvalue = 2;
    saveconfig();
}

Uint8 loadconfig()
{
    int b, vmaj, vmin;
    Uint8 c;
    FILE *fp;
    fp = fopen(KOPSCFGFILE, "rb");
    fread(&vmaj, 4, 1, fp);
    fread(&vmin, 4, 1, fp);
    for (b = 0; b < MAXPLAYERS; b++)
    {
	fread(&c, 1, 1, fp);
	fread(&plr[b].k_up, 4, 1, fp);
	fread(&plr[b].k_left, 4, 1, fp);
	fread(&plr[b].k_right, 4, 1, fp);
	fread(&plr[b].k_fire1, 4, 1, fp);
	fread(&plr[b].k_fire2, 4, 1, fp);
    }
    fread(&players, 1, 1, fp);
    fread(&bots, 1, 1, fp);
    fread(&parallaxdraw, 1, 1, fp);
    fread(&slots, 1, 1, fp);
    fread(&s_sdevice, 1, 1, fp);
    fread(&s_samprate, 4, 1, fp);
//      fread(&s_interp,1,1,fp);
    fread(&s_shortchn, 1, 1, fp);
    fread(&s_longchn, 1, 1, fp);
    fread(&s_musvol, 1, 1, fp);
    fread(&s_effvol, 1, 1, fp);
    fread(&gravvalue, 1, 1, fp);
    fclose(fp);
    snd = s_sdevice;
    if ((vmaj != versmaj) || (vmin != versmin) || (players == 0)
        || (players > 4) || (parallaxdraw > 1) || (slots < 2)
        || (bots == 0) || (bots > 4)
        || (slots > 16)) {
        return (0);
    }
    if ((s_sdevice > 1) || (s_samprate > 44100)
        || /*(s_interp>1)|| */ (s_shortchn == 0) ||
        (s_shortchn > 16) || (s_longchn == 0) || (s_longchn > 16)
        || (s_musvol > 64) || (s_effvol > 64) || s_samprate < 8000) {
        return (0);
    }
    return (1);
}

void loadlevel(char *filename)
{
#define MAXLEVELREVISION  1
    unsigned long levellen, backgrlen, transplen;
    Uint32 pkpiclen, pkbacklen, pktransplen, revision;
    Uint8 orgsig[8] = { 'K', 'O', 'P', 'S', 'L', 'E', 'V', 26 };
    Uint8 sig[8];
    UTIL_FILE *fp;
    Uint8 *packed;
    int err, a, b, w;

    fp = util_fopen(filename);
    if (fp == NULL) {
        jerror("LOADLEVEL: No file", 1);
    }
    util_fread(&sig, 8, 1, fp);
    util_fread(&revision, 4, 1, fp);
    util_fread(&pkpiclen, 4, 1, fp);
    util_fread(&levw, 4, 1, fp);
    util_fread(&levh, 4, 1, fp);
    util_fread(&pkbacklen, 4, 1, fp);
    util_fread(&backw, 4, 1, fp);
    util_fread(&backh, 4, 1, fp);
    util_fread(&pktransplen, 4, 1, fp);

    if (memcmp(&sig, &orgsig, 8) != 0) {
        jerror("LOADLEVEL.signaturefailure", 26);
    }
    if (revision > MAXLEVELREVISION) {
        jerror("LOADLEVEL.too_new_level_revision", MAXLEVELREVISION);
    }

    levellen = levw * levh;
    level = (Uint8 *) realloc(level, levellen);
    if (level == NULL) {
        jerror("LOADLEVEL.realloc/level: No mem", 3);
    }
    backgrlen = backw * backh;
    backgr = (Uint8 *) realloc(backgr, backgrlen);
    if (backgr == NULL) {
        jerror("LOADLEVEL.realloc/backgr: No mem", 4);
    }

    packed = (Uint8 *) malloc(pkpiclen);
    if (packed == NULL) {
        jerror("LOADLEVEL.pkmalloc: No mem", 2);
    }
    util_fread(packed, pkpiclen, 1, fp);
    err = uncompress(level, &levellen, packed, pkpiclen);
    if (err != Z_OK) {
        jerror("LOADLEVEL.uncompress1 ", err);
    }
    if (levellen != (unsigned) (levw * levh)) {
        jerror("LOADLEVEL.size_error.l", levellen);
    }

    packed = (Uint8 *) realloc(packed, pkbacklen);
    if (packed == NULL) {
        jerror("LOADLEVEL.realloc/pkback: No mem", 5);
    }
    util_fread(packed, pkbacklen, 1, fp);
    err = uncompress(backgr, &backgrlen, packed, pkbacklen);
    if (err != Z_OK) {
        jerror("LOADLEVEL.uncompress2 ", err);
    }
    if (backgrlen != (unsigned) (backw * backh)) {
        jerror("LOADLEVEL.size_error.b", backgrlen);
    }

    packed = (Uint8 *) realloc(packed, pktransplen);
    if (packed == NULL) {
        jerror("LOADLEVEL.realloc/pktransp: No mem", 6);
    }
    util_fread(packed, pktransplen, 1, fp);
    transplen = 65536;
    err = uncompress(transp, &transplen, packed, pktransplen);
    if (err != Z_OK) {
        jerror("LOADLEVEL.uncompress3/t ", err);
    }
    if (transplen != 65536) {
        jerror("LOADLEVEL.size_error.t", transplen);
    }

    free(packed);
    packed = NULL;

    mask = (Uint8 *) realloc(mask, levellen);
    if (mask == NULL) {
        jerror("LOADLEVEL.realloc/mask: No mem", 7);
    }
    for (a = 0; (unsigned) a < levellen; a++) {
        *(mask + a) = (Uint8) ((*(level + a)) ? 0 : 0xff);
    }
    wave = (int *) realloc(wave, levw * sizeof(int));
    if (wave == NULL) {
        jerror("LOADLEVEL.realloc/wave: No mem", 8);
    }

    /*  pickammo spots */
    w = 0;
    for (a = 0; (unsigned) a < levellen; a++) {
        if (*(level + a) == 9) {
            w++;
        }
    }
    if (w == 0) {
        jerror("LOADLEVEL.pickammo: No pickammo spot pixels on level", 11);
    }
    pickammospots = w;
    pickammo = (PICKAMMO *) realloc(pickammo, sizeof(PICKAMMO) * pickammospots);
    if (pickammo == NULL) {
        jerror("LOADLEVEL.realloc/pickammo: No mem", w);
    }
    w = 0;
    for (b = 0; b < levh; b++) {
	for (a = 0; a < levw; a++) {
	    if (*(level + b * levw + a) == 9)
            {
		pickammo[w].active = 0;
		pickammo[w].type = 0;
		pickammo[w].timeleft = 0;
		pickammo[w].bullets = 0;
		pickammo[w].xp = a;
		pickammo[w].yp = b;
		pickammo[w].x = (a * FIXS);
		pickammo[w].y = (b * FIXS);
		pickammo[w].cnt = 0;
		pickammo[w].orgy = (b * FIXS);
		*(level + b * levw + a) = *(level + b * levw + a - 1);
		*(mask + b * levw + a) = *(mask + b * levw + a - 1);
		w++;
	    }
	}
}

    /*  water wave points */
    w = 0;
    for (a = 0; (unsigned) a < levellen; a++) {
        if (*(level + a) == 7) {
            w++;
        }
    }
    waterwaves = w;
    if (w > 0) {
	water = (WATER *) realloc(water, sizeof(WATER) * w);
	if (water == NULL) {
	    jerror("LOADLEVEL.realloc/water: No mem", w);
    }
	w = 0;
	for (b = 0; b < levh; b++) {
	    for (a = 0; a < levw; a++) {
		if (*(level + b * levw + a) == 7)
                {
		    water[w].x = (short) a;
		    water[w].y = (short) b;
		    w++;
		}
        }
    }
    }

    /*  base pixels */
    w = 0;
    for (a = 0; (unsigned) a < levellen; a++) {
        if ((*(level + a) >= 10) && (*(level + a) <= 14)) {
            w++;
        }
    }
    if (w == 0) {
        jerror("LOADLEVEL.base: No base pixels on level", 10);
    }
    basepixels = w;
    base = (BASE *) realloc(base, sizeof(BASE) * basepixels);
    if (base == NULL) {
        jerror("LOADLEVEL.realloc/base: No mem", basepixels);
    }
    w = 0;
    for (b = levh - 1; b >= 0; b--) {
	for (a = levw - 1; a >= 0; a--) {
	    if ((*(level + b * levw + a) >= 10)
		&& (*(level + b * levw + a) <= 14))
            {
		base[w].x = (short) a;
		base[w].y = (short) b;
		base[w].c = *(level + b * levw + a);
		w++;
	    }
	}
    }

    /*  water pattern */
    for (a = 0; (unsigned) a < levellen; a++)
    {
	if ((*(level + a) > 0) && (*(level + a) < 8))
        {
	    *(mask + a) = wttrans[(a / levw + wttable[(a % levw) % WTTABLELEN]) % WTTRANSLEN];
	    *(level + a) = 1;
	}
    }

    /*  clear colors 127-255 and 15 */
    for (a = 0; (unsigned) a < levellen; a++) {
        if (*(level + a) > 127) {
            *(level + a) = 0;
        }
        if (*(level + a) == 15) {
            *(level + a) = 0;
        }
    }

    util_fclose(fp);
    jloadpcxpalpartdest(filename, 0, 255);
}

void loadships(char *filename)
{
    unsigned long unpklen;
    Uint32 unpackedsize, packedsize;
    Uint8 *pk, *unpk;
    int err;
    UTIL_FILE *fp;
    int index;

    fp = util_fopen(filename);
    if (fp == NULL) {
        jerror("LOADSHIPS: No file", 1);
    }

    util_fread(&packedsize, 4, 1, fp);
    util_fread(&unpackedsize, 4, 1, fp);
    unpklen = unpackedsize;

    unpk = (Uint8 *) malloc(unpklen);
    if (unpk == NULL) {
        jerror("LOADSHIPS.unpk: No mem", 6);
    }
    pk = (Uint8 *) malloc(packedsize);
    if (pk == NULL) {
        jerror("LOADSHIPS.pk: No mem", 7);
    }

    util_fread(pk, packedsize, 1, fp);
    util_fclose(fp);

    err = uncompress(unpk, &unpklen, pk, packedsize);
    if (err != Z_OK) {
        jerror("LOADSHIPS.uncompress ", err);
    }
    if (unpklen != unpackedsize) {
        jerror("LOADSHIPS.size_error", unpklen);
    }
    free(pk);
    pk = NULL;

    for(index = 0; index < (MAXPLAYERS + MAXBOTS); ++index)
    {
	plr[index].ship = (Uint8 *) realloc(plr[index].ship, SHIPTOTSIZE);
	if (plr[index].ship == NULL)
        {
            jerror("LOADSHIPS.p0r: No mem", 2);
        }
    }

#if 0  /* Removed by smo. */
    plr[0].ship = (Uint8 *) realloc(plr[0].ship, SHIPTOTSIZE);
    if (plr[0].ship == NULL)
	jerror("LOADSHIPS.p0r: No mem", 2);
    plr[1].ship = (Uint8 *) realloc(plr[1].ship, SHIPTOTSIZE);
    if (plr[1].ship == NULL)
	jerror("LOADSHIPS.p1r: No mem", 3);
    plr[2].ship = (Uint8 *) realloc(plr[2].ship, SHIPTOTSIZE);
    if (plr[2].ship == NULL)
	jerror("LOADSHIPS.p2r: No mem", 4);
    plr[3].ship = (Uint8 *) realloc(plr[3].ship, SHIPTOTSIZE);
    if (plr[3].ship == NULL)
	jerror("LOADSHIPS.p3r: No mem", 5);

    //  memcpy(plr[0].ship,unpk+SHIPTOTSIZE*0,SHIPTOTSIZE);
    //  memcpy(plr[1].ship,unpk+SHIPTOTSIZE*1,SHIPTOTSIZE);
    //  memcpy(plr[2].ship,unpk+SHIPTOTSIZE*2,SHIPTOTSIZE);
    //  memcpy(plr[3].ship,unpk+SHIPTOTSIZE*3,SHIPTOTSIZE);
#endif /* Removed by smo. */

    memcpy(plr[0].ship, unpk, 65536);
    memcpy(plr[2].ship, unpk + 65536, 65536);
    memcpy(plr[3].ship, unpk + 65536 * 2, 65536);
    memcpy(plr[1].ship, unpk + 65536 * 3, 65536);

    memcpy(plr[7].ship, unpk, 65536);
    memcpy(plr[6].ship, unpk + 65536, 65536);
    memcpy(plr[5].ship, unpk + 65536 * 2, 65536);
    memcpy(plr[4].ship, unpk + 65536 * 3, 65536);

    free(unpk);
    unpk = NULL;
}

void loadgfx(char *filename)
{
    Uint8 fontstr[FONTCHARS] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" "()%#@$!\"/\\+-_?=*.,><:;\345\344\366\260\261\262\333\372\371\376 "
    };
    Uint8 bigfontstr[BIGFONTCHRS] =
	{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ\345\344\3660123456789" "!\"?:.;,+-=@\334\337()/#%$\\_*<>\260\261\262\333\372\371\376 "
        };
    unsigned long unplen;
    Uint32 pkgfxlen, gfxlen, gfxw, gfxh;
    UTIL_FILE *fp;
    Uint8 *packed, *unp;
    Uint8 *tmp;
    int err, a, b, ltr;
    PIC bfont;

    fp = util_fopen(filename);
    if (fp == NULL) {
        jerror("LOADGFX: No file", 1);
    }
    util_fread(&pkgfxlen, 4, 1, fp);
    util_fread(&gfxw, 4, 1, fp);
    util_fread(&gfxh, 4, 1, fp);

    unplen = gfxw * gfxh;
    unp = (Uint8 *) malloc(unplen);
    if (unp == NULL) {
        jerror("LOADGFX.malloc.unp: No mem", 4);
    }

    packed = (Uint8 *) malloc(pkgfxlen);
    if (packed == NULL) {
        jerror("LOADGFX.malloc.pk: No mem", 3);
    }
    util_fread(packed, pkgfxlen, 1, fp);
    err = uncompress(unp, &unplen, packed, pkgfxlen);
    if (err != Z_OK) {
        jerror("LOADGFX.uncompress", err);
    }
    //if (unplen!=(unsigned)gfxw*gfxh) jerror("LOADGFX.size_error",unplen);
    free(packed);
    packed = NULL;

    gfxlen = 0;
    for (a = 0; a < BULLETTYPES; a++) {
        gfxlen += bullet[a].siz;
    }
    for (a = 0; a < BULLETBOXTS; a++) {
        gfxlen += bulletbox[a].siz;
    }
    for (a = 0; a < OTHERGFX; a++) {
        gfxlen += othgfx[a].siz;
    }
    gfx = (Uint8 *) realloc(gfx, gfxlen);
    if (gfx == NULL) {
        jerror("LOADGFX.realloc.gfx: No mem", 2);
    }

    /*  booms */
    for (b = 0; b < boomanim[0].frames; b++) {
	for (a = 0; a < boomanim[0].h; a++) {	/* 1st */
	    memcpy(boomanim[0].pic + b * boomanim[0].siz +
		   a * boomanim[0].w, unp + (a + 1) * gfxw + b * (boomanim[0].w + 1) + 18, boomanim[0].w);
    }
    }
    for (b = 0; b < boomanim[1].frames; b++) {
	for (a = 0; a < boomanim[1].h; a++) {	/* 2nd */
	    memcpy(boomanim[1].pic + b * boomanim[1].siz +
		   a * boomanim[1].w, unp + (a + 18) * gfxw + b * (boomanim[1].w + 1) + 18, boomanim[1].w);
    }
    }
    for (b = 0; b < boomanim[2].frames; b++) {
	for (a = 0; a < boomanim[2].h; a++) {	/* 3rd */
	    memcpy(boomanim[2].pic + b * boomanim[2].siz +
		   a * boomanim[2].w, unp + (a + 43) * gfxw + b * (boomanim[2].w + 1) + 18, boomanim[2].w);
    }
    }
    for (b = 0; b < 8; b++) {
	for (a = 0; a < boomanim[3].h; a++) {	/* 4th/row1 */
	    memcpy(boomanim[3].pic + b * boomanim[3].siz +
		   a * boomanim[3].w, unp + (a + 76) * gfxw + b * (boomanim[3].w + 1) + 18, boomanim[3].w);
    }
    }
    for (b = 0; b < 8; b++) {
	for (a = 0; a < boomanim[3].h; a++) {	/* 4th/row2 */
	    memcpy(boomanim[3].pic + (b + 8) * boomanim[3].siz +
		   a * boomanim[3].w, unp + (a + 125) * gfxw + b * (boomanim[3].w + 1) + 18, boomanim[3].w);
    }
    }
    for (b = 0; b < boomanim[4].frames; b++) {
	for (a = 0; a < boomanim[4].h; a++) {	/* 5th */
	    memcpy(boomanim[4].pic + b * boomanim[4].siz +
		   a * boomanim[4].w, unp + (a + 193) * gfxw + b * (boomanim[4].w + 1) + 67, boomanim[4].w);
    }
    }
    for (b = 0; b < boomanim[5].frames; b++) {
	for (a = 0; a < boomanim[5].h; a++) {	/* 6th */
	    memcpy(boomanim[5].pic + b * boomanim[5].siz +
		   a * boomanim[5].w, unp + (a + 218) * gfxw + b * (boomanim[5].w + 1) + 67, boomanim[5].w);
    }
    }

    tmp = gfx;
    /*  bullets */
    bullet[0].pic = tmp;
    tmp += bullet[0].siz;	// tuhnu
    jcgetblock(1, 1, bullet[0].w, bullet[0].h, gfxw, unp, bullet[0].pic);
    bullet[1].pic = tmp;
    tmp += bullet[1].siz;	// peagun
    jcgetblock(5, 1, bullet[1].w, bullet[1].h, gfxw, unp, bullet[1].pic);
    bullet[2].pic = tmp;
    tmp += bullet[2].siz;	// sling
    jcgetblock(9, 1, bullet[2].w, bullet[2].h, gfxw, unp, bullet[2].pic);
    bullet[3].pic = tmp;
    tmp += bullet[3].siz;	// big peagun
    jcgetblock(13, 1, bullet[3].w, bullet[3].h, gfxw, unp, bullet[3].pic);
    bullet[4].pic = tmp;
    tmp += bullet[4].siz;	// big sling
    jcgetblock(11, 5, bullet[4].w, bullet[4].h, gfxw, unp, bullet[4].pic);
    bullet[5].pic = tmp;
    tmp += bullet[5].siz;	// blastgun
    jcgetblock(11, 11, bullet[5].w, bullet[5].h, gfxw, unp, bullet[5].pic);
    bullet[6].pic = tmp;
    tmp += bullet[6].siz;	// mini blaster
    jcgetblock(11, 17, bullet[6].w, bullet[6].h, gfxw, unp, bullet[6].pic);
    bullet[7].pic = tmp;
    tmp += bullet[7].siz;	// blaster
    jcgetblock(11, 23, bullet[7].w, bullet[7].h, gfxw, unp, bullet[7].pic);
    bullet[8].pic = tmp;
    tmp += bullet[8].siz;	// big blaster
    jcgetblock(1, 85, bullet[8].w, bullet[8].h, gfxw, unp, bullet[8].pic);
    bullet[9].pic = tmp;
    tmp += bullet[9].siz;	// gas blaster
    jcgetblock(1, 93, bullet[9].w, bullet[9].h, gfxw, unp, bullet[9].pic);
    bullet[10].pic = tmp;
    tmp += bullet[10].siz;	// mega blaster
    jcgetblock(1, 101, bullet[10].w, bullet[10].h, gfxw, unp, bullet[10].pic);
    bullet[11].pic = tmp;
    tmp += bullet[11].siz;	// mine, 6 frames
    for (a = 0; a < 6; a++) {
        jcgetblock(a * 13 + 86, 174, bullet[11].w, bullet[11].h,
                   gfxw, unp, bullet[11].pic + a * bullet[11].w * bullet[11].h);
    }
    bullet[12].pic = tmp;
    tmp += bullet[12].siz;	// heatseeker
    jcgetblock(9, 85, bullet[12].w, bullet[12].h, gfxw, unp, bullet[12].pic);
    bullet[13].pic = tmp;
    tmp += bullet[13].siz;	// morning star
    jcgetblock(9, 93, bullet[13].w, bullet[13].h, gfxw, unp, bullet[13].pic);
    bullet[14].pic = tmp;
    tmp += bullet[14].siz;	// sensor jammer
    jcgetblock(9, 101, bullet[14].w, bullet[14].h, gfxw, unp, bullet[14].pic);
    bullet[15].pic = tmp;
    tmp += bullet[15].siz;	// no weapon
    bullet[16].pic = tmp;
    tmp += bullet[16].siz;	// pyrotechnics
    jcgetblock(190, 174, bullet[16].w, bullet[16].h, gfxw, unp, bullet[16].pic);
    bullet[17].pic = tmp;
    tmp += bullet[17].siz;	// toxic waste
    jcgetblock(210, 174, bullet[17].w, bullet[17].h, gfxw, unp, bullet[17].pic);
    bullet[18].pic = tmp;
    tmp += bullet[18].siz;	// toaster
    jcgetblock(230, 174, bullet[18].w, bullet[18].h, gfxw, unp, bullet[18].pic);
    bullet[19].pic = tmp;
    tmp += bullet[19].siz;	// rear sling
    jcgetblock(250, 174, bullet[19].w, bullet[19].h, gfxw, unp, bullet[19].pic);
    bullet[20].pic = tmp;
    tmp += bullet[20].siz;	// ring of fire
    bullet[21].pic = tmp;
    tmp += bullet[21].siz;	// crawler, 3 frames
    for (a = 0; a < 3; a++) {
        jcgetblock(278, a * 6 + 174, bullet[21].w, bullet[21].h,
                   gfxw, unp, bullet[21].pic + a * bullet[21].w * bullet[21].h);
    }
    bullet[22].pic = tmp;
    tmp += bullet[22].siz;	// gravity flow
    jcgetblock(294, 174, bullet[22].w, bullet[22].h, gfxw, unp, bullet[22].pic);
    bullet[23].pic = tmp;
    tmp += bullet[23].siz;	// bomb
    jcgetblock(314, 174, bullet[23].w, bullet[23].h, gfxw, unp, bullet[23].pic);
    bullet[24].pic = tmp;
    tmp += bullet[24].siz;	// lash
    jcgetblock(334, 174, bullet[24].w, bullet[24].h, gfxw, unp, bullet[24].pic);
    bullet[25].pic = tmp;
    tmp += bullet[25].siz;	// ultrathrust
    bullet[26].pic = tmp;
    tmp += bullet[26].siz;	// retroboost
    bullet[27].pic = tmp;
    tmp += bullet[27].siz;	// hyperspace projector
    bullet[28].pic = tmp;
    tmp += bullet[28].siz;	// plasma ball
    bullet[29].pic = tmp;
    tmp += bullet[29].siz;	// pulse laser
    bullet[30].pic = tmp;
    tmp += bullet[30].siz;	// delayed blast
    bullet[31].pic = tmp;
    tmp += bullet[31].siz;	// flipper virus
    jcgetblock(426, 174, bullet[31].w, bullet[31].h, gfxw, unp, bullet[31].pic);
    bullet[32].pic = tmp;
    tmp += bullet[32].siz;	// signal jammer
    jcgetblock(446, 174, bullet[32].w, bullet[32].h, gfxw, unp, bullet[32].pic);
    bullet[33].pic = tmp;
    tmp += bullet[33].siz;	// next weapon
    bullet[34].pic = tmp;
    tmp += bullet[34].siz;	// tuhnuruisku
    bullet[35].pic = tmp;
    tmp += bullet[35].siz;	// hyper blaster
    bullet[36].pic = tmp;
    tmp += bullet[36].siz;	// tractor beam
    bullet[37].pic = tmp;
    tmp += bullet[37].siz;	// twin laser
    bullet[38].pic = tmp;
    tmp += bullet[38].siz;	// super pyrotechnics
    bullet[39].pic = tmp;
    tmp += bullet[39].siz;	// spiller
    //  bullet[\260\260].pic=tmp; tmp+=bullet[\260\260].siz;  //
    //  jcgetblock(\260\260\260,\260\260\260,bullet[\260\260].w,bullet[\260\260].h,gfxw,unp,bullet[\260\260].pic);

    /*  bulletboxes */
    bulletbox[0].pic = tmp;
    tmp += bulletbox[0].siz;	// tuhnu
    jcgetblock(1, 5, bulletbox[0].w, bulletbox[0].h, gfxw, unp, bulletbox[0].pic);
    bulletbox[1].pic = tmp;
    tmp += bulletbox[1].siz;	// peagun
    jcgetblock(1, 15, bulletbox[1].w, bulletbox[1].h, gfxw, unp, bulletbox[1].pic);
    bulletbox[2].pic = tmp;
    tmp += bulletbox[2].siz;	// sling
    jcgetblock(1, 25, bulletbox[2].w, bulletbox[2].h, gfxw, unp, bulletbox[2].pic);
    bulletbox[3].pic = tmp;
    tmp += bulletbox[3].siz;	// big peagun
    jcgetblock(1, 35, bulletbox[3].w, bulletbox[3].h, gfxw, unp, bulletbox[3].pic);
    bulletbox[4].pic = tmp;
    tmp += bulletbox[4].siz;	// big sling
    jcgetblock(1, 45, bulletbox[4].w, bulletbox[4].h, gfxw, unp, bulletbox[4].pic);
    bulletbox[5].pic = tmp;
    tmp += bulletbox[5].siz;	// blastgun
    jcgetblock(1, 55, bulletbox[5].w, bulletbox[5].h, gfxw, unp, bulletbox[5].pic);
    bulletbox[6].pic = tmp;
    tmp += bulletbox[6].siz;	// mini blaster
    jcgetblock(1, 65, bulletbox[6].w, bulletbox[6].h, gfxw, unp, bulletbox[6].pic);
    bulletbox[7].pic = tmp;
    tmp += bulletbox[7].siz;	// blaster
    jcgetblock(1, 75, bulletbox[7].w, bulletbox[7].h, gfxw, unp, bulletbox[7].pic);
    bulletbox[8].pic = tmp;
    tmp += bulletbox[8].siz;	// big blaster
    jcgetblock(1, 109, bulletbox[8].w, bulletbox[8].h, gfxw, unp, bulletbox[8].pic);
    bulletbox[9].pic = tmp;
    tmp += bulletbox[9].siz;	// gas blaster
    jcgetblock(1, 121, bulletbox[9].w, bulletbox[9].h, gfxw, unp, bulletbox[9].pic);
    bulletbox[10].pic = tmp;
    tmp += bulletbox[10].siz;	// mega blaster
    jcgetblock(1, 133, bulletbox[10].w, bulletbox[10].h, gfxw, unp, bulletbox[10].pic);
    bulletbox[11].pic = tmp;
    tmp += bulletbox[11].siz;	// mine
    jcgetblock(164, 174, bulletbox[11].w, bulletbox[11].h, gfxw, unp, bulletbox[11].pic);
    bulletbox[12].pic = tmp;
    tmp += bulletbox[12].siz;	// heatseeker
    jcgetblock(1, 145, bulletbox[12].w, bulletbox[12].h, gfxw, unp, bulletbox[12].pic);
    bulletbox[13].pic = tmp;
    tmp += bulletbox[13].siz;	// morning star
    jcgetblock(1, 157, bulletbox[13].w, bulletbox[13].h, gfxw, unp, bulletbox[13].pic);
    bulletbox[14].pic = tmp;
    tmp += bulletbox[14].siz;	// sensor jammer
    jcgetblock(1, 169, bulletbox[14].w, bulletbox[14].h, gfxw, unp, bulletbox[14].pic);
    bulletbox[15].pic = tmp;
    tmp += bulletbox[15].siz;	// no weapon
    jcgetblock(178, 174, bulletbox[15].w, bulletbox[15].h, gfxw, unp, bulletbox[15].pic);
    bulletbox[16].pic = tmp;
    tmp += bulletbox[16].siz;	// pyrotechnics
    jcgetblock(198, 174, bulletbox[16].w, bulletbox[16].h, gfxw, unp, bulletbox[16].pic);
    bulletbox[17].pic = tmp;
    tmp += bulletbox[17].siz;	// toxic waste
    jcgetblock(218, 174, bulletbox[17].w, bulletbox[17].h, gfxw, unp, bulletbox[17].pic);
    bulletbox[18].pic = tmp;
    tmp += bulletbox[18].siz;	// toaster
    jcgetblock(238, 174, bulletbox[18].w, bulletbox[18].h, gfxw, unp, bulletbox[18].pic);
    bulletbox[19].pic = tmp;
    tmp += bulletbox[19].siz;	// rear sling
    jcgetblock(256, 174, bulletbox[19].w, bulletbox[19].h, gfxw, unp, bulletbox[19].pic);
    bulletbox[20].pic = tmp;
    tmp += bulletbox[20].siz;	// ring of fire
    jcgetblock(266, 174, bulletbox[20].w, bulletbox[20].h, gfxw, unp, bulletbox[20].pic);
    bulletbox[21].pic = tmp;
    tmp += bulletbox[21].siz;	// crawler
    jcgetblock(284, 174, bulletbox[21].w, bulletbox[21].h, gfxw, unp, bulletbox[21].pic);
    bulletbox[22].pic = tmp;
    tmp += bulletbox[22].siz;	// gravity flow
    jcgetblock(302, 174, bulletbox[22].w, bulletbox[22].h, gfxw, unp, bulletbox[22].pic);
    bulletbox[23].pic = tmp;
    tmp += bulletbox[23].siz;	// bomb
    jcgetblock(322, 174, bulletbox[23].w, bulletbox[23].h, gfxw, unp, bulletbox[23].pic);
    bulletbox[24].pic = tmp;
    tmp += bulletbox[24].siz;	// lash
    jcgetblock(342, 174, bulletbox[24].w, bulletbox[24].h, gfxw, unp, bulletbox[24].pic);
    bulletbox[25].pic = tmp;
    tmp += bulletbox[25].siz;	// ultrathrust
    jcgetblock(354, 174, bulletbox[25].w, bulletbox[25].h, gfxw, unp, bulletbox[25].pic);
    bulletbox[26].pic = tmp;
    tmp += bulletbox[26].siz;	// retroboost
    jcgetblock(366, 174, bulletbox[26].w, bulletbox[26].h, gfxw, unp, bulletbox[26].pic);
    bulletbox[27].pic = tmp;
    tmp += bulletbox[27].siz;	// hyperspace projector
    jcgetblock(378, 174, bulletbox[27].w, bulletbox[27].h, gfxw, unp, bulletbox[27].pic);
    bulletbox[28].pic = tmp;
    tmp += bulletbox[28].siz;	// plasma ball
    jcgetblock(390, 174, bulletbox[28].w, bulletbox[28].h, gfxw, unp, bulletbox[28].pic);
    bulletbox[29].pic = tmp;
    tmp += bulletbox[29].siz;	// pulse laser
    jcgetblock(402, 174, bulletbox[29].w, bulletbox[29].h, gfxw, unp, bulletbox[29].pic);
    bulletbox[30].pic = tmp;
    tmp += bulletbox[30].siz;	// delayed blast
    jcgetblock(414, 174, bulletbox[30].w, bulletbox[30].h, gfxw, unp, bulletbox[30].pic);
    bulletbox[31].pic = tmp;
    tmp += bulletbox[31].siz;	// flipper virus
    jcgetblock(434, 174, bulletbox[31].w, bulletbox[31].h, gfxw, unp, bulletbox[31].pic);
    bulletbox[32].pic = tmp;
    tmp += bulletbox[32].siz;	// signal jammer
    jcgetblock(454, 174, bulletbox[32].w, bulletbox[32].h, gfxw, unp, bulletbox[32].pic);
    bulletbox[33].pic = tmp;
    tmp += bulletbox[33].siz;	// next weapon
    jcgetblock(466, 174, bulletbox[33].w, bulletbox[33].h, gfxw, unp, bulletbox[33].pic);
    bulletbox[34].pic = tmp;
    tmp += bulletbox[34].siz;	// tuhnuruisku
    jcgetblock(478, 174, bulletbox[34].w, bulletbox[34].h, gfxw, unp, bulletbox[34].pic);
    bulletbox[35].pic = tmp;
    tmp += bulletbox[35].siz;	// hyper blaster
    jcgetblock(490, 174, bulletbox[35].w, bulletbox[35].h, gfxw, unp, bulletbox[35].pic);
    bulletbox[36].pic = tmp;
    tmp += bulletbox[36].siz;	// tractor beam
    jcgetblock(502, 174, bulletbox[36].w, bulletbox[36].h, gfxw, unp, bulletbox[36].pic);
    bulletbox[37].pic = tmp;
    tmp += bulletbox[37].siz;	// twin laser
    jcgetblock(514, 174, bulletbox[37].w, bulletbox[37].h, gfxw, unp, bulletbox[37].pic);
    bulletbox[38].pic = tmp;
    tmp += bulletbox[38].siz;	// super pyrotechnics
    jcgetblock(526, 174, bulletbox[38].w, bulletbox[38].h, gfxw, unp, bulletbox[38].pic);
    bulletbox[39].pic = tmp;
    tmp += bulletbox[39].siz;	// spiller
    jcgetblock(538, 174, bulletbox[39].w, bulletbox[39].h, gfxw, unp, bulletbox[39].pic);
    //  bulletbox[\260\260].pic=tmp; tmp+=bulletbox[\260\260].siz;
    //  jcgetblock(\260\260\260,\260\260\260,bulletbox[\260\260].w,bulletbox[\260\260].h,gfxw,unp,bulletbox[\260\260].pic);

    /*  other gfx (0,1) */
    othgfx[0].pic = tmp;
    tmp += othgfx[0].siz;
    jcgetblock(18, 174, othgfx[0].w, othgfx[0].h, gfxw, unp, othgfx[0].pic);
    othgfx[1].pic = tmp;
    tmp += othgfx[1].siz;
    jcgetblock(67, 174, othgfx[1].w, othgfx[1].h, gfxw, unp, othgfx[1].pic);

    /*  small font */
    font.w = 8;
    font.h = 10;
    font.p = (Uint8 *) realloc(font.p, FONTCHARS * font.w * font.h);
    if (font.p == NULL) {
        jerror("LOADGFX.realloc.font: No mem", 9);
    }
    ltr = 0;
    for (b = 0; b < 5; b++)
    {
	for (a = 0; a < ((b < 4) ? 15 : 9); a++)
        {
	    jcgetblock(410 + a * 9, 76 + b * 11, font.w, font.h, gfxw, unp, font.p + ltr * font.w * font.h);
	    ltr++;
	}
    }
    for (b = 0; b < 256; b++)
    {
	font.o[b] = font.p + 68 * font.w * font.h;	// space
    for (a = 0; a < FONTCHARS; a++) {
        if (fontstr[a] == toupper(b)) {
            font.o[b] = font.p + a * font.w * font.h;
        }
	}
    }

    util_fclose(fp);
    free(unp);
    unp = NULL;

    /*  big fonts */
    util_loadpcxpi(BIGFONTFILE, &bfont);
    bigfont[0].w = 24;
    bigfont[0].h = 24;
    bigfont[1].w = 24;
    bigfont[1].h = 24;
    bigfont[0].p = (Uint8 *) realloc(bigfont[0].p, BIGFONTCHRS * bigfont[0].w * bigfont[0].h);
    if (bigfont[0].p == NULL) {
        jerror("LOADGFX.realloc.bigfont.0: No mem", 10);
    }
    bigfont[1].p = (Uint8 *) realloc(bigfont[1].p, BIGFONTCHRS * bigfont[1].w * bigfont[1].h);
    if (bigfont[1].p == NULL) {
        jerror("LOADGFX.realloc.bigfont.10: No mem", 10);
    }
    ltr = 0;
    for (b = 0; b < 3; b++)
    {
	for (a = 0; a < ((b < 2) ? 25 : 21); a++)
        {
	    jcgetblock(1 + a * 25, 1 + b * 25, bigfont[0].w, bigfont[0].h,
		       bfont.w, bfont.pic, bigfont[0].p + ltr * bigfont[0].w * bigfont[0].h);
	    jcgetblock(1 + a * 25, 76 + b * 25, bigfont[1].w, bigfont[1].h,
		       bfont.w, bfont.pic, bigfont[1].p + ltr * bigfont[1].w * bigfont[1].h);
	    ltr++;
	}
    }
    for (b = 0; b < 256; b++)
    {
	bigfont[0].o[b] = bigfont[0].p + 70 * bigfont[0].w * bigfont[0].h;	// space
	bigfont[1].o[b] = bigfont[1].p + 70 * bigfont[1].w * bigfont[1].h;
	for (a = 0; a < BIGFONTCHRS; a++)
        {
	    if (bigfontstr[a] == toupper(b))
            {
		bigfont[0].o[b] = bigfont[0].p + a * bigfont[0].w * bigfont[0].h;
		bigfont[1].o[b] = bigfont[1].p + a * bigfont[1].w * bigfont[1].h;
	    }
	}
    }

    /*  other gfx (2) */
    othgfx[2].pic = tmp;
    tmp += othgfx[2].siz;
    for (b = 0; b < 4; b++) {
        for (a = 0; a < 16; a++) {
            jcgetblock(a * 33 + 1, b * 33 + 151, othgfx[2].w, othgfx[2].h,
                       bfont.w, bfont.pic, othgfx[2].pic + (b * 16 + a) * othgfx[2].w * othgfx[2].h);
        }
    }
    util_freepi(&bfont);
}
