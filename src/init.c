/* init.c
 * Copyright 1996,2001,2003 Jetro Lauha.
 *
 * $Id: init.c,v 1.1 2003/04/15 18:15:49 tonic Exp $
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
#include <io.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#ifndef _WIN32
#include <dirent.h>
#include <sys/stat.h>
#endif
#include "wport.h"
#include "global.h"
#include "timer.h"
#include "load.h"
#include "sound.h"
#include "init.h"

char musicfile[80];

#define INIT_WAIT_FOR_KEYPRESS
//#define INIT_SHOWLEVELLIST

static void uninitGraphics(void);

void initGraphics()
{
    char sdf[1024];

    /* Initialize the SDL library */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        sprintf(sdf, "Couldn't initialize SDL: %s\n", SDL_GetError());
        jerror(sdf, EXIT_FAILURE);
    }

    /* Clean up on exit */
    atexit(uninitGraphics);

    /* 8-bit surface for game to write directly to */
    screen = SDL_CreateRGBSurface(0,
                                  X_RESOLUTION, Y_RESOLUTION,
                                  8, 0, 0, 0, 0);

    if (screen == NULL) {
        sprintf(sdf, "Couldn't create surface\n");
        jerror(sdf, EXIT_FAILURE);
    }

    /* Initialize the display in a 640x480 mode */
    window = SDL_CreateWindow("KOPS", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              screen->w, screen->h,
                              SDL_WINDOW_OPENGL);

    if (window == NULL) {
        sprintf(sdf, "Couldn't set 640x480x8 video mode: %s\n", SDL_GetError());
        jerror(sdf, EXIT_FAILURE);
    }

    SDL_Surface *icon = SDL_LoadBMP("icon.bmp");
    if (icon) {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }
    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer,
                             screen->w, screen->h);

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == NULL) {
        sprintf(sdf, "Couldn't create renderer\n");
        jerror(sdf, EXIT_FAILURE);
    }

    /* Surface matching display colors */
    windowSurface = SDL_CreateRGBSurface(0,
                                         screen->w, screen->h,
                                         32, 0, 0, 0, 0);

    if (windowSurface == NULL) {
        sprintf(sdf, "Couldn't create window surface\n");
        jerror(sdf, EXIT_FAILURE);
    }

    /* And finally a texture also matching display colors */
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                screen->w,
                                screen->h);

    if (texture == NULL) {
        sprintf(sdf, "Couldn't create texture\n");
        jerror(sdf, EXIT_FAILURE);
    }
}

static void uninitGraphics(void)
{
    if (texture) {
        SDL_DestroyTexture(texture);
    }
    if (windowSurface) {
        SDL_FreeSurface(windowSurface);
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    if (screen) {
        SDL_FreeSurface(screen);
    }
    SDL_Quit();
}

void initgfxstructures()
{
    int a;
    boomanim[0].w = 16;
    boomanim[0].h = 16;
    boomanim[0].siz = boomanim[0].w * boomanim[0].h;
    boomanim[0].frames = 16;
    boomanim[0].pic = (Uint8 *)malloc(boomanim[0].frames * boomanim[0].siz);
    boomanim[1].w = 24;
    boomanim[1].h = 24;
    boomanim[1].siz = boomanim[1].w * boomanim[1].h;
    boomanim[1].frames = 16;
    boomanim[1].pic = (Uint8 *)malloc(boomanim[1].frames * boomanim[1].siz);
    boomanim[2].w = 32;
    boomanim[2].h = 32;
    boomanim[2].siz = boomanim[2].w * boomanim[2].h;
    boomanim[2].frames = 16;
    boomanim[2].pic = (Uint8 *)malloc(boomanim[2].frames * boomanim[2].siz);
    boomanim[3].w = 48;
    boomanim[3].h = 48;
    boomanim[3].siz = boomanim[3].w * boomanim[3].h;
    boomanim[3].frames = 16;
    boomanim[3].pic = (Uint8 *)malloc(boomanim[3].frames * boomanim[3].siz);
    boomanim[4].w = 24;
    boomanim[4].h = 24;
    boomanim[4].siz = boomanim[4].w * boomanim[4].h;
    boomanim[4].frames = 16;
    boomanim[4].pic = (Uint8 *)malloc(boomanim[4].frames * boomanim[4].siz);
    boomanim[5].w = 32;
    boomanim[5].h = 32;
    boomanim[5].siz = boomanim[5].w * boomanim[5].h;
    boomanim[5].frames = 16;
    boomanim[5].pic = (Uint8 *)malloc(boomanim[5].frames * boomanim[5].siz);

    for (a = 0; a < BOOMTYPES; a++) {
        memset(boomanim[a].pic, 0, boomanim[a].frames * boomanim[a].siz);
    }
    for (a = 0; a < BULLETTYPES; a++) {
        bullet[a].frames = 1;
    }
    for (a = 0; a < BULLETBOXTS; a++) {
        bulletbox[a].prob = 0;
    }

    bullet[0].w = 3;
    bullet[0].h = 3; // tuhnu
    bullet[1].w = 3;
    bullet[1].h = 3; // peagun
    bullet[2].w = 3;
    bullet[2].h = 3; // sling
    bullet[3].w = 3;
    bullet[3].h = 3; // big peagun
    bullet[4].w = 5;
    bullet[4].h = 5; // big sling
    bullet[5].w = 5;
    bullet[5].h = 5; // blastgun
    bullet[6].w = 5;
    bullet[6].h = 5; // mini blaster
    bullet[7].w = 5;
    bullet[7].h = 5; // blaster
    bullet[8].w = 7;
    bullet[8].h = 7; // big blaster
    bullet[9].w = 7;
    bullet[9].h = 7; // gas blaster
    bullet[10].w = 7;
    bullet[10].h = 7; // mega blaster
    bullet[11].w = 12;
    bullet[11].h = 14; // mine
    bullet[11].frames = 6;
    bullet[12].w = 7;
    bullet[12].h = 7; // heatseeker
    bullet[13].w = 7;
    bullet[13].h = 7; // morning star
    bullet[14].w = 7;
    bullet[14].h = 7; // sensor jammer
    bullet[15].w = 0;
    bullet[15].h = 0; // no weapon
    bullet[16].w = 7;
    bullet[16].h = 7; // pyrotechnics
    bullet[17].w = 7;
    bullet[17].h = 7; // toxic waste
    bullet[18].w = 7;
    bullet[18].h = 7; // toaster
    bullet[19].w = 5;
    bullet[19].h = 5; // rear sling
    bullet[20].w = 0;
    bullet[20].h = 0; // ring of fire
    bullet[21].w = 5;
    bullet[21].h = 5; // crawler
    bullet[21].frames = 3;
    bullet[22].w = 7;
    bullet[22].h = 7; // gravity flow
    bullet[23].w = 7;
    bullet[23].h = 7; // bomb
    bullet[24].w = 7;
    bullet[24].h = 7; // lash
    bullet[25].w = 0;
    bullet[25].h = 0; // ultrathrust
    bullet[26].w = 0;
    bullet[26].h = 0; // retroboost
    bullet[27].w = 0;
    bullet[27].h = 0; // hyperspace projector
    bullet[28].w = 0;
    bullet[28].h = 0; // plasma ball
    bullet[29].w = 0;
    bullet[29].h = 0; // pulse laser
    bullet[30].w = 0;
    bullet[30].h = 0; // delayed blast
    bullet[31].w = 7;
    bullet[31].h = 7; // flipper virus
    bullet[32].w = 7;
    bullet[32].h = 7; // signal jammer
    bullet[33].w = 0;
    bullet[33].h = 0; // next weapon
    bullet[34].w = 0;
    bullet[34].h = 0; // tuhnuruisku
    bullet[35].w = 0;
    bullet[35].h = 0; // hyper blaster
    bullet[36].w = 0;
    bullet[36].h = 0; // tractor beam
    bullet[37].w = 0;
    bullet[37].h = 0; // twin laser
    bullet[38].w = 0;
    bullet[38].h = 0; // super pyrotechnics
    bullet[39].w = 0;
    bullet[39].h = 0; // spiller
    //  bullet[\260\260].w=\260; bullet[\260\260].h=\260;   //
    for (a = 0; a < BULLETTYPES; a++) {
        bullet[a].siz = bullet[a].w * bullet[a].h * bullet[a].frames;
    }

    bulletbox[0].w = 9;
    bulletbox[0].h = 9; // tuhnu
    bulletbox[1].w = 9;
    bulletbox[1].h = 9; // peagun
    bulletbox[2].w = 9;
    bulletbox[2].h = 9; // sling
    bulletbox[3].w = 9;
    bulletbox[3].h = 9; // big peagun
    bulletbox[4].w = 9;
    bulletbox[4].h = 9; // big sling
    bulletbox[5].w = 9;
    bulletbox[5].h = 9; // blastgun
    bulletbox[6].w = 9;
    bulletbox[6].h = 9; // mini blaster
    bulletbox[7].w = 9;
    bulletbox[7].h = 9; // blaster
    bulletbox[8].w = 11;
    bulletbox[8].h = 11; // big blaster
    bulletbox[9].w = 11;
    bulletbox[9].h = 11; // gas blaster
    bulletbox[10].w = 11;
    bulletbox[10].h = 11; // mega blaster
    bulletbox[11].w = 13;
    bulletbox[11].h = 13; // mine
    bulletbox[12].w = 11;
    bulletbox[12].h = 11; // heatseeker
    bulletbox[13].w = 11;
    bulletbox[13].h = 11; // morning star
    bulletbox[14].w = 11;
    bulletbox[14].h = 11; // sensor jammer
    bulletbox[15].w = 11;
    bulletbox[15].h = 11; // no weapon
    bulletbox[16].w = 11;
    bulletbox[16].h = 11; // pyrotechnics
    bulletbox[17].w = 11;
    bulletbox[17].h = 11; // toxic waste
    bulletbox[18].w = 11;
    bulletbox[18].h = 11; // toaster
    bulletbox[19].w = 9;
    bulletbox[19].h = 9; // rear sling
    bulletbox[20].w = 11;
    bulletbox[20].h = 11; // ring of fire
    bulletbox[21].w = 9;
    bulletbox[21].h = 9; // crawler
    bulletbox[22].w = 11;
    bulletbox[22].h = 11; // gravity flow
    bulletbox[23].w = 11;
    bulletbox[23].h = 11; // bomb
    bulletbox[24].w = 11;
    bulletbox[24].h = 11; // lash
    bulletbox[25].w = 11;
    bulletbox[25].h = 11; // ultrathrust
    bulletbox[26].w = 11;
    bulletbox[26].h = 11; // retroboost
    bulletbox[27].w = 11;
    bulletbox[27].h = 11; // hyperspace projector
    bulletbox[28].w = 11;
    bulletbox[28].h = 11; // plasma ball
    bulletbox[29].w = 11;
    bulletbox[29].h = 11; // pulse laser
    bulletbox[30].w = 11;
    bulletbox[30].h = 11; // delayed blast
    bulletbox[31].w = 11;
    bulletbox[31].h = 11; // flipper virus
    bulletbox[32].w = 11;
    bulletbox[32].h = 11; // signal jammer
    bulletbox[33].w = 11;
    bulletbox[33].h = 11; // next weapon
    bulletbox[34].w = 11;
    bulletbox[34].h = 11; // tuhnuruisku
    bulletbox[35].w = 11;
    bulletbox[35].h = 11; // hyper blaster
    bulletbox[36].w = 11;
    bulletbox[36].h = 11; // tractor beam
    bulletbox[37].w = 11;
    bulletbox[37].h = 11; // twin laser
    bulletbox[38].w = 11;
    bulletbox[38].h = 11; // super pyrotechnics
    bulletbox[39].w = 11;
    bulletbox[39].h = 11; // spiller
    //  bulletbox[\260\260].w=; bulletbox[\260\260].h=;     //
    for (a = 0; a < BULLETBOXTS; a++) {
        bulletbox[a].siz = bulletbox[a].w * bulletbox[a].h;
    }

    bulletbox[0].prob = 30;   // tuhnu
    bulletbox[1].prob = 10;   // peagun
    bulletbox[2].prob = 55;   // sling
    bulletbox[3].prob = 50;   // big peagun
    bulletbox[4].prob = 45;   // big sling
    bulletbox[5].prob = 40;   // blastgun
    bulletbox[6].prob = 35;   // mini blaster
    bulletbox[7].prob = 20;   // blaster
    bulletbox[8].prob = 15;   // big blaster
    bulletbox[9].prob = 30;   // gas blaster
    bulletbox[10].prob = 15;  // mega blaster
    bulletbox[11].prob = 35;  // mine
    bulletbox[12].prob = 60;  // heatseeker
    bulletbox[13].prob = 50;  // morning star
    bulletbox[14].prob = 70;  // sensor jammer
    bulletbox[15].prob = 50;  // no weapon
    bulletbox[16].prob = 20;  // pyrotechnics
    bulletbox[17].prob = 60;  // toxic waste
    bulletbox[18].prob = 50;  // toaster
    bulletbox[19].prob = 40;  // rear sling
    bulletbox[20].prob = 50;  // ring of fire
    bulletbox[21].prob = 50;  // crawler
    bulletbox[22].prob = 50;  // gravity flow
    bulletbox[23].prob = 60;  // bomb
    bulletbox[24].prob = 50;  // lash
    bulletbox[25].prob = 65;  // ultrathrust
    bulletbox[26].prob = 65;  // retroboost
    bulletbox[27].prob = 25;  // hyperspace projector
    bulletbox[28].prob = 40;  // plasma ball
    bulletbox[29].prob = 60;  // pulse laser
    bulletbox[30].prob = 30;  // delayed blast
    bulletbox[31].prob = 60;  // flipper virus
    bulletbox[32].prob = 40;  // signal jammer
    bulletbox[33].prob = 120; // next weapon
#ifdef REG
    bulletbox[34].prob = 15; // tuhnuruisku
    bulletbox[35].prob = 5;  // hyper blaster
    bulletbox[36].prob = 25; // tractor beam
    bulletbox[37].prob = 13; // twin laser
    bulletbox[38].prob = 5;  // super pyrotechnics
#endif
    bulletbox[39].prob = 20; // spiller
    //  bulletbox[].prob=;     //

    othgfx[0].w = 48;
    othgfx[0].h = 48;
    othgfx[0].frames = 1; // glowball1
    othgfx[1].w = 18;
    othgfx[1].h = 18;
    othgfx[1].frames = 1; // glowball2
    othgfx[2].w = 32;
    othgfx[2].h = 32;
    othgfx[2].frames = 64; // menuship
    for (a = 0; a < OTHERGFX; a++) {
        othgfx[a].siz = othgfx[a].w * othgfx[a].h * othgfx[a].frames;
    }

    strncpy(&wnames[0][0], "tuhnu", WEAPON_NAME_LENGTH);
    strncpy(&wnames[1][0], "peagun", WEAPON_NAME_LENGTH);
    strncpy(&wnames[2][0], "sling", WEAPON_NAME_LENGTH);
    strncpy(&wnames[3][0], "big peagun", WEAPON_NAME_LENGTH);
    strncpy(&wnames[4][0], "big sling", WEAPON_NAME_LENGTH);
    strncpy(&wnames[5][0], "blastgun", WEAPON_NAME_LENGTH);
    strncpy(&wnames[6][0], "mini blaster", WEAPON_NAME_LENGTH);
    strncpy(&wnames[7][0], "blaster", WEAPON_NAME_LENGTH);
    strncpy(&wnames[8][0], "big blaster", WEAPON_NAME_LENGTH);
    strncpy(&wnames[9][0], "gas blaster", WEAPON_NAME_LENGTH);
    strncpy(&wnames[10][0], "mega blaster", WEAPON_NAME_LENGTH);
    strncpy(&wnames[11][0], "mine", WEAPON_NAME_LENGTH);
    strncpy(&wnames[12][0], "heatseeker", WEAPON_NAME_LENGTH);
    strncpy(&wnames[13][0], "morning star", WEAPON_NAME_LENGTH);
    strncpy(&wnames[14][0], "sensor jammer", WEAPON_NAME_LENGTH);
    strncpy(&wnames[15][0], "no weapon", WEAPON_NAME_LENGTH);
    strncpy(&wnames[16][0], "pyrotechnics", WEAPON_NAME_LENGTH);
    strncpy(&wnames[17][0], "toxic waste", WEAPON_NAME_LENGTH);
    strncpy(&wnames[18][0], "toaster", WEAPON_NAME_LENGTH);
    strncpy(&wnames[19][0], "rear sling", WEAPON_NAME_LENGTH);
    strncpy(&wnames[20][0], "ring of fire", WEAPON_NAME_LENGTH);
    strncpy(&wnames[21][0], "crawler", WEAPON_NAME_LENGTH);
    strncpy(&wnames[22][0], "gravity flow", WEAPON_NAME_LENGTH);
    strncpy(&wnames[23][0], "bomb", WEAPON_NAME_LENGTH);
    strncpy(&wnames[24][0], "lash", WEAPON_NAME_LENGTH);
    strncpy(&wnames[25][0], "ultrathrust", WEAPON_NAME_LENGTH);
    strncpy(&wnames[26][0], "retroboost", WEAPON_NAME_LENGTH);
    strncpy(&wnames[27][0], "hyperspace projector", WEAPON_NAME_LENGTH);
    strncpy(&wnames[28][0], "plasma ball", WEAPON_NAME_LENGTH);
    strncpy(&wnames[29][0], "pulse laser", WEAPON_NAME_LENGTH);
    strncpy(&wnames[30][0], "delayed blast", WEAPON_NAME_LENGTH);
    strncpy(&wnames[31][0], "flipper virus", WEAPON_NAME_LENGTH);
    strncpy(&wnames[32][0], "signal jammer", WEAPON_NAME_LENGTH);
    strncpy(&wnames[34][0], "tuhnuruisku", WEAPON_NAME_LENGTH);
    strncpy(&wnames[35][0], "hyper blaster", WEAPON_NAME_LENGTH);
    strncpy(&wnames[36][0], "tractor beam", WEAPON_NAME_LENGTH);
    strncpy(&wnames[37][0], "twin laser", WEAPON_NAME_LENGTH);
    strncpy(&wnames[38][0], "super pyrotechnics", WEAPON_NAME_LENGTH);
    strncpy(&wnames[39][0], "spiller", WEAPON_NAME_LENGTH);
    //  strcpy(&wnames[\260\260][0],"");

    /*  pickammo total probability value */
    totalprob = 0;
    for (a = 0; a < BULLETBOXTS; a++) {
        totalprob += bulletbox[a].prob;
    }

    /*  FONT INITS are mixed with loading of the fonts! */
}

void initlevels()
{
#ifdef REG
#ifdef _WIN32
    struct _finddata_t fileinfo;
    intptr_t hFile;
#else
    DIR *dirp;
    struct dirent *dp;
    struct stat filestat;
#endif
#endif
    int a;
    levels = 0;
    alllevels = LEVELS;
    for (a = 0; a < MAXLEVELS; a++) {
        strncpy(levall[a], "\334", LEVEL_NAME_LENGTH);
    }
#ifndef REG
    strncpy(levall[0], "\337steppe.kil", LEVEL_NAME_LENGTH);
    strncpy(levall[1], "\337coldone.kil", LEVEL_NAME_LENGTH);
    strncpy(levall[2], "\337cave.kil", LEVEL_NAME_LENGTH);
    strncpy(levall[3], "\337blobs.kil", LEVEL_NAME_LENGTH);
#ifndef ONEDISK
    strncpy(levall[4], "\337tubes.kil", LEVEL_NAME_LENGTH);
#endif
#else
    strncpy(levall[0], "\337steppe.kil", LEVEL_NAME_LENGTH);
    strncpy(levall[1], "\337coldone.kil", LEVEL_NAME_LENGTH);
    strncpy(levall[2], "\337cave.kil", LEVEL_NAME_LENGTH);
    strncpy(levall[3], "\337rude.kil", LEVEL_NAME_LENGTH);
    strncpy(levall[4], "\337scape.kil", LEVEL_NAME_LENGTH);
    strncpy(levall[5], "\337blobs.kil", LEVEL_NAME_LENGTH);
    strncpy(levall[6], "\337sad.kil", LEVEL_NAME_LENGTH);
    strncpy(levall[7], "\337tubes.kil", LEVEL_NAME_LENGTH);
    strncpy(levall[8], "\337space.kil", LEVEL_NAME_LENGTH);
#endif
    levels = 1;
    strncpy(levlist[0], levall[rand() % LEVELS], LEVEL_NAME_LENGTH);
#ifdef REG
    /*  if registered version, scan for external levels (*.KOP) */
#ifdef _WIN32
    intptr_t rc = hFile = _findfirst("*.KOP", &fileinfo);
    if (rc >= 0)
        rc = 0;
    while ((rc == 0) && (alllevels < MAXLEVELS)) {
        strcat(levall[alllevels], fileinfo.name);
        rc = _findnext(hFile, &fileinfo);
        alllevels++;
    }
    _findclose(hFile);
#else
#define LEVEL_DIRECTORY "."
    dirp = opendir(LEVEL_DIRECTORY);
    if (dirp) {
        for (a = 0; ((dp = readdir(dirp)) != NULL) && (alllevels < MAXLEVELS);) {
            char fullname[PATH_MAX];

            snprintf(fullname, sizeof(fullname), "%s/%s", LEVEL_DIRECTORY, dp->d_name);
            if (stat(fullname, &filestat)) {
                continue;
            }
            if (S_ISREG(filestat.st_mode) && ((toupper((int)dp->d_name[strlen(dp->d_name) - 4]) == '.') && (toupper((int)dp->d_name[strlen(dp->d_name) - 3]) == 'K') && (toupper((int)dp->d_name[strlen(dp->d_name) - 2]) == 'O') && (toupper((int)dp->d_name[strlen(dp->d_name) - 1]) == 'P'))) {
                strcat(levall[alllevels], dp->d_name);
                alllevels++;
            }
        }
    }
    closedir(dirp);
#undef LEVEL_DIRECTORY
#endif
#endif
#ifdef INIT_SHOWLEVELLIST
    printf("\nList of available levels:\n  ");
    rc = 0;
    for (a = 0; a < alllevels; a++) {
        printf("%14s", &levall[a]);
        rc++;
        if (rc > 4) {
            rc = 0;
            printf("\n  ");
        }
    }
    printf("\n ...");
#endif
}

void parsemusicinitfile(char *filename)
{
    char str[80], dest[80], tmp[80], musfileok = 0;
    UTIL_FILE *fp;
    int ti;

    memset(str, 0, 80);
    mgcount = 0;
    strcpy(musicfile, "");
    fp = util_fopent(filename); //, "rt");
    if (fp == NULL) {
        jerror("Music init file not found!", 1);
    }
    m_startmenu = m_status = m_endgame = m_info = 0;
    while ((!util_feof(fp)) && (str[0] != '#')) {
        u_fscanf1(fp, "%[^\n]", str);
        if (str[0] != ';') {
            sscanf(str, "%[^=]", dest);
            if (!stricmp(dest, "FILE")) {
                sscanf(str, "%[^=]%[=]%s", tmp, tmp, musicfile);
                musfileok = 1;
            }
            if (!stricmp(dest, "STARTMENU")) {
                sscanf(str, "%[^=]%[=]%d", tmp, tmp, &ti);
                m_startmenu = ti;
            }
            if (!stricmp(dest, "STATUS")) {
                sscanf(str, "%[^=]%[=]%d", tmp, tmp, &ti);
                m_status = ti;
            }
            if (!stricmp(dest, "ENDGAME")) {
                sscanf(str, "%[^=]%[=]%d", tmp, tmp, &ti);
                m_endgame = ti;
            }
            if (!stricmp(dest, "INFO")) {
                sscanf(str, "%[^=]%[=]%d", tmp, tmp, &ti);
                m_info = ti;
            }
            if (!stricmp(dest, "GAME")) {
                sscanf(str, "%[^=]%[=]%d", tmp, tmp, &ti);
                m_game[mgcount] = (Uint8)ti;
                mgcount++;
                if (mgcount >= MAXGMUS) {
                    jerror("Too many game background music orders.", MAXGMUS);
                }
            }
        }
        u_fscanf1(fp, "%[\n]", tmp);
    }
    util_fclose(fp);
    if (mgcount == 0) {
        jerror("No game background music orders.", 1);
    }
    if ((musfileok == 0) || (strlen(musicfile) == 0)) {
        jerror("No music file.", 1);
    }
}

void initsound()
{
    strncpy(&effnames[0][0], "$tuhnu.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[1][0], "$peagun.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[2][0], "$sling.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[3][0], "$bigpeag.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[4][0], "$bigslin.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[5][0], "$blastgu.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[6][0], "$minibla.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[7][0], "$blaster.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[8][0], "$bigblas.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[9][0], "$gasblas.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[10][0], "$megabla.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[11][0], "$mine.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[12][0], "$heatsee.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[13][0], "$heatset.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[14][0], "$mornsta.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[15][0], "$sensorj.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[16][0], "$pyrotec.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[17][0], "$toxicwa.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[18][0], "$toaster.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[19][0], "$rearsli.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[20][0], "$ringoff.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[21][0], "$crawler.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[22][0], "$gravflo.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[23][0], "$bomb.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[24][0], "$lash.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[25][0], "$lasht.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[26][0], "$ultrath.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[27][0], "$retrobo.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[28][0], "$hypersp.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[29][0], "$plasmab.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[30][0], "$pulslas.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[31][0], "$delblas.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[32][0], "$flipper.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[33][0], "$signalj.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[34][0], "$collect.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[35][0], "$menu1.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[36][0], "$menu2.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[37][0], "$menu3.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[38][0], "$b_minib.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[39][0], "$b_bigbl.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[40][0], "$b_blast.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[41][0], "$b_blgun.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[42][0], "$b_bomb.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[43][0], "$b_delbl.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[44][0], "$b_gasbl.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[45][0], "$b_megab.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[46][0], "$b_ship.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[47][0], "$shiphit.smp", EFFECT_NAME_LENGTH);
    strncpy(&effnames[48][0], "$gndhit.smp", EFFECT_NAME_LENGTH);
    parsemusicinitfile(minitfile);
    sound_init();
}

void configerrorexit()
{
    printf(" N\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
    printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
    printf("Use KOPSETUP.EXE to create configuration.\r\n\r\n");
    exit(2);
}

void initgravity()
{
    switch (gravvalue) {
    case 0:
        gravity = 0;
        watergrav = -2;
        ammogravity = 4;
        break;
    case 1:
        gravity = 5;
        watergrav = -10;
        ammogravity = 13;
        break;
    case 2:
        gravity = 10;
        watergrav = -20;
        ammogravity = 27;
        break;
    case 3:
        gravity = 18;
        watergrav = -36;
        ammogravity = 38;
        break;
    case 4:
        gravity = 30;
        watergrav = -60;
        ammogravity = 50;
        break;
    }
}

void init()
{
    int a, err;
    FILE *fp;
    int vmaj, vmin;

    memset(plr, 0, (MAXPLAYERS + MAXBOTS) * sizeof(PLAYER));
    memset(crap, 0, MAXCRAP * sizeof(CRAP));
    memset(ammo, 0, MAXAMMO * sizeof(AMMO));
    memset(boom, 0, MAXBOOMS * sizeof(BOOM));
    memset(boomanim, 0, BOOMTYPES * sizeof(BOOMANIM));
    memset(bullet, 0, BULLETTYPES * sizeof(BULLET));
    memset(bulletbox, 0, BULLETBOXTS * sizeof(BULLETBOX));
    memset(othgfx, 0, OTHERGFX * sizeof(OTHGFX));

    if (fileexists(KOPSCFGFILE) == 0) {
        newconfig();
    }

    fp = fopen(KOPSCFGFILE, "rb");
    fread(&vmaj, 4, 1, fp);
    fread(&vmin, 4, 1, fp);
    fclose(fp);
    if ((vmaj != versmaj) || (vmin != versmin)) {
        newconfig();
    }

    initGraphics();

    for (a = 0; a < (MAXPLAYERS + MAXBOTS); a++) {
        plr[a].active = 0;
    }

    util_finit();
    err = util_fopenlib(KOPSJFLFILE);
    if (err < 0) {
        printf("\nLibrary error: ");
        switch (err) {
        case -1:
            printf("no free library place\n");
            break;
        case -2:
            printf("can\'t open library file\n");
            break;
        case -3:
            printf("invalid library signature\n");
            break;
        case -4:
            printf("no files in library\n");
            break;
        case -5:
            printf("not enough memory\n");
            break;
        }
        printf("\n Initialization failed. \n\n");
        exit(10);
    }
    printf("init: library open\n");

    printf("init: trigonometry tables");
    for (a = 0; a < SCT; a++) {
        sint[a] = (int)floor(sin(a * PI / (SCT / 2)) * FIXS);
        cost[a] = (int)floor(cos(a * PI / (SCT / 2)) * FIXS);
    }
    printf(" calculated\n");

    transp = (Uint8 *)malloc(256 * 256);
    srand(util_gettick());

    printf("init: gfx structures");
    initgfxstructures();
    printf(" initialized\n");

    printf("levels: ");
    initlevels();
    printf("ok\n");

    printf("graphics:");
    loadgfx(KOPSGFXFILE);
    printf(" ..main gfx..");
    loadships(KOPSSHIPFILE);
    printf(" ..ships.\n");

    if (!loadconfig()) {
        newconfig();
    }

    if (kludgesnd) {
        snd = 0;
    }

    printf("sound: ");
    if (snd) {
        initsound();
        printf("..ok.\n");
    } else {
        printf("ok (off - no sound)\n");
    }

    for (a = 0; a < players; a++) {
        initplayer(a);
    }

    for (a = 0; a < bots; a++) {
        initplayer(MAXPLAYERS + a);
    }

    gamepause = 0;
    initgravity();
    initweapons();

    settimer(timer_rate);
}

void deinit()
{
    int a;
    freetimer();
    sound_deinit();

    for (a = 0; a < players; a++) {
        deinitplayer(a);
    }

    for (a = 0; a < bots; a++) {
        deinitplayer(MAXPLAYERS + a);
    }

    free(minitfile);
    minitfile = NULL;
    free(level);
    level = NULL;
    free(mask);
    mask = NULL;
    free(backgr);
    backgr = NULL;
    if (waterwaves > 0) {
        free(water);
        water = NULL;
    }
    free(wave);
    wave = NULL;
    free(base);
    base = NULL;
    free(pickammo);
    pickammo = NULL;
    free(transp);
    transp = NULL;
    free(gfx);
    gfx = NULL;
    for (a = 0; a < (MAXPLAYERS + MAXBOTS); a++) {
        free(plr[a].ship);
        plr[a].ship = NULL;
    }
    for (a = 0; a < BOOMTYPES; a++) {
        free(boomanim[a].pic);
        boomanim[a].pic = NULL;
    }
    free(font.p);
    font.p = NULL;
    free(bigfont[0].p);
    bigfont[0].p = NULL;
    free(bigfont[1].p);
    bigfont[1].p = NULL;
    util_fcloselib(KOPSJFLFILE);
}

void initplayer(int nro)
{
    int a;

    plr[nro].scr = (Uint8 *)malloc(WINW * WINH);

    if (plr[nro].scr == NULL) {
        jerror("INITPLAYER.malloc/scr: No mem", WINW * WINH);
    }

    plr[nro].tmp = (Uint8 *)malloc(WINW * WINH);

    if (plr[nro].tmp == NULL) {
        jerror("INITPLAYER.malloc/tmp: No mem", WINW * WINH);
    }

    memset(plr[nro].tmp, 0, WINW * WINH);

    plr[nro].x = 0;
    plr[nro].y = 0;
    plr[nro].xp = 0;
    plr[nro].yp = 0;
    plr[nro].wx = 0;
    plr[nro].wy = 0;
    plr[nro].xi = 0;
    plr[nro].yi = 0;
    plr[nro].dir = 0;
    plr[nro].blink = 0;
    plr[nro].ground = 0;
    plr[nro].energy = STARTENERGY << FIXP;
    plr[nro].sensorjam = 0;
    plr[nro].hyperspac = 0;
    plr[nro].flip = 0;
    plr[nro].signal = 0;
    plr[nro].activeslot = 0;
    plr[nro].nextslot = 1;
    plr[nro].slotchangepause = 0;
    plr[nro].controltype = 0;

    for (a = 0; a < MAXSLOTS; a++) {
        plr[nro].slots[a] = W_NOWEAPON;
        plr[nro].bullets[a] = 0;
        //    plr[nro].slots[a]=rand()%24+9;
        //    plr[nro].bullets[a]=rand()+10;
    }
    plr[nro].ammo1 = W_PEAGUN;
    plr[nro].ammo2 = plr[nro].slots[plr[nro].activeslot];
    plr[nro].active = 1;
}

void resetplayer(int nro)
{
    int a;

    memset(plr[nro].tmp, 0, WINW * WINH);

    plr[nro].x = 0;
    plr[nro].y = 0;
    plr[nro].xp = 0;
    plr[nro].yp = 0;
    plr[nro].wx = 0;
    plr[nro].wy = 0;
    plr[nro].xi = 0;
    plr[nro].yi = 0;
    plr[nro].dir = 0;
    plr[nro].blink = 0;
    plr[nro].ground = 0;
    plr[nro].energy = STARTENERGY << FIXP;
    plr[nro].sensorjam = 0;
    plr[nro].hyperspac = 0;
    plr[nro].flip = 0;
    plr[nro].signal = 0;
    plr[nro].activeslot = 0;
    plr[nro].nextslot = 1;
    plr[nro].slotchangepause = 0;
    plr[nro].controltype = 0;
    for (a = 0; a < MAXSLOTS; a++) {
        plr[nro].slots[a] = W_NOWEAPON;
        plr[nro].bullets[a] = 0;
    }
    plr[nro].ammo1 = W_PEAGUN;
    plr[nro].ammo2 = plr[nro].slots[plr[nro].activeslot];
    plr[nro].active = 1;
}

void resetplayer2(int nro)
{ /* doesn't reset WEAPONS, except hyperspace */
    int a;
    memset(plr[nro].tmp, 0, WINW * WINH);
    plr[nro].x = 0;
    plr[nro].y = 0;
    plr[nro].xp = 0;
    plr[nro].yp = 0;
    plr[nro].wx = 0;
    plr[nro].wy = 0;
    plr[nro].xi = 0;
    plr[nro].yi = 0;
    plr[nro].dir = 0;
    plr[nro].blink = 0;
    plr[nro].ground = 0;
    plr[nro].energy = STARTENERGY << FIXP;
    plr[nro].sensorjam = 0;
    plr[nro].hyperspac = 0;
    plr[nro].flip = 0;
    plr[nro].signal = 0;
    plr[nro].slotchangepause = 0;
    plr[nro].controltype = 0;
    for (a = 0; a < MAXSLOTS; a++) {
        if (plr[nro].slots[a] == W_HYPERSPAC) {
            plr[nro].slots[a] = W_NOWEAPON;
            plr[nro].bullets[a] = 0;
        }
    }
    plr[nro].ammo2 = plr[nro].slots[plr[nro].activeslot];
    plr[nro].active = 1;
}

void deinitplayer(int nro)
{
    free(plr[nro].scr);
    plr[nro].scr = NULL;
    free(plr[nro].tmp);
    plr[nro].tmp = NULL;
    plr[nro].active = 0;
}
