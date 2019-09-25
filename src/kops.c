/* kops.c
 * Copyright 1996,1997,2001,2003 Jetro Lauha.
 *
 * $Id: kops.c,v 1.1 2003/04/15 18:15:49 tonic Exp $
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
#include <malloc.h>
#include "wport.h"
#include "global.h"
#include "load.h"
#include "init.h"
#include "draw.h"
#include "game.h"
#include "menu.h"
#include "kops-bot/reactive-agent/util.h"


/*****  GLOBAL variables  ************************************************/
#include "version.h"
int windowed = 0;
char *copyright = "Copyright 1996,1997,2001,2003 Jetro Lauha.";
volatile unsigned long int framecounter;
volatile Uint8 gamepause;
unsigned int cnt;
Uint8 *level, *backgr, *mask;	/* level & background image and mask */
Uint8 parallaxdraw;		/* parallax on/off */
int levw, levh, backw, backh;	/* level+background width & height */
int sint[SCT], cost[SCT];	/* sin & cos tables */
Uint8 *gfx;			/* gfx (bullets, boxes etc) */
Uint8 *transp;			/* transparency table */
WATER *water;			/* water waves on level */
BASE *base;			/* bases */
PICKAMMO *pickammo;		/* pickammos */
int *wave;			/* wave image */
Uint8 slots;			/* max amount of active weapon slots */
int waterwaves, basepixels;	/* water waves on level */
int basepixcount;		/* base regeneration counter */
char wnames[BULLETTYPES][21];	/* weapon names */
int pickammospots;		/* pickammo spots */
int totalprob;			/* ammo probabilities */
int alllevels, levels;		/* amount of all&choosed levels */
char levall[MAXLEVELS][14];	/* all levels found */
char levlist[MAXLEVELS][14];	/* choosed levels */
char effnames[SNDEFFECTS][13];	/* sound effect file names */
Uint8 m_game[MAXGMUS];		/* music starting orders info */
int m_startmenu, m_status, m_endgame, m_info;
int mgcount;			/* count of game musics */
char *minitfile;		/* music init file */
Uint8 s_sdevice = 1, s_interp = 0, s_shortchn = 8, s_longchn = 8, s_musvol = 32, s_effvol = 63;
unsigned int s_samprate = 22050;	/* sound infos */
Uint8 snd = 1;			/* sounds on/off (1/0) */
Uint8 kludgesnd = 0;		/* force sound disable as an command line parameter */
int wins[MAXPLAYERS+MAXBOTS];   /* win table */
int round;			/* round number */
int totalframes, drawnframes;	/* for average FPS */
Uint8 gamewindowcolorrand;
Uint8 players;
Uint8 bots;
Uint8 testmode = 0;
int nextammo, nextcrap, nextboom;
Uint8 wttrans[WTTRANSLEN] = { 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1 };
Uint8 wttable[WTTABLELEN] = { 0, 0, 0, 0, 1, 1, 2, 3, 4, 4, 5, 5, 5, 5, 4, 4, 3, 2, 1, 1 };
PLAYER plr[MAXPLAYERS+MAXBOTS];
CRAP crap[MAXCRAP];
AMMO ammo[MAXAMMO];
BOOM boom[MAXBOOMS];
BOOMANIM boomanim[BOOMTYPES];
BULLET bullet[BULLETTYPES];
BULLETBOX bulletbox[BULLETBOXTS];
OTHGFX othgfx[OTHERGFX];
FONT font, bigfont[2];
Uint8 gravvalue = 2;
int gravity = 10;
int watergrav = -20;
int ammogravity = 27;

Config config;

/*************************************************************************/

int main(int argc, char *argv[])
{
    FILE* log;
    int count, tmp, a = 0;
    char mtmp, jatko, higrav = 0;

    config.debug_lvl   = 0;

    log = fopen("kops.log", "w");
    if ( log == NULL )
    {
	perror("Run log file 'kops.log' open for writing failed.\n");
	exit(EXIT_FAILURE);
    }

    config.debug_log   = log;
    config.access_log  = log;
    config.error_log   = log;

    if (argc < 1)
    {
	printf("Program file name is unknown.\n\n");
	exit(1);
    }
    /*  parse parameters */
    mtmp = 0;			/* music initfile flag */
    for (count = 1; count < argc; count++)
    {
	if ((argv[count][0] != '\\') && (argv[count][0] != '/')
	    && (argv[count][0] != '-'))
        {
	    printf("\n\nValid parameters:\n\n");
#ifdef REG
	    printf("  /mMYFILE.KMU  Use external music init file (REG)\n");
#endif
	    printf("  /t            Test mode (free mem, average fps, etc..)\n");
	    printf("  /c#           Enter a funny number.\n");
	    printf("\n");
	    exit(0);
	}
	switch (toupper(argv[count][1]))
        {
#ifdef REG
            case 'M':
                mtmp = 1;
                minitfile = strdup(&argv[count][2]);
                break;
#endif
            case 'T':
                testmode = 1;
                break;
            case 'D':
                config.debug_lvl = atoi(&argv[count][2]);
                break;
            case 'C':
                tmp = atoi(&argv[count][2]);
                switch (tmp)
                {
                    case 1:
                        windowed = 1;
                        break;
                    case 2:
                        kludgesnd = 1;
                        break;
                    case 4:
                        printf("\n\"BlackHole\" activated.\n");
                        higrav = 1;
                        break;
                    default:
                        printf("\nInvalid code.\n");
                }
                break;
            default:
                argv[count][0] = '?';
                count--;
	}
    }
    if (mtmp == 0)
	minitfile = strdup(MINITFILE);

    init();

    atexit((void (*)(void)) deinit);

    if (higrav)
    {
	gravvalue = 4;
	initgravity();
    }

    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY / 2, SDL_DEFAULT_REPEAT_INTERVAL * 2);

    while (mainmenu() != 6)
    {
	for (a = 0; a < players; a++)
        {
	    resetplayer(a);
	}
	for (a = 0; a < bots; a++)
        {
	    resetplayer(MAXPLAYERS + a);
	}

	for (a = 0; a < (MAXPLAYERS + MAXBOTS); a++)
        {
	    wins[a] = 0;
	}

	for (a = 0; a < levels; a++)
        {
	    round = a + 1;
	    jloadpcxpalpartdest((char *) (&levlist[a]) + 1, 0, 127);
	    jloadpcxpalpartdest(KOPSGFXFILE, 128, 191);
	    jloadpcxpalpartdest(KOPSSHIPFILE, 192, 255);
	    jclearpal();
	    jclearstart();
	    jinitreal();
	    gamewindowbackgrounds();
	    loadlevel((char *) (&levlist[a]) + 1);
	    SDL_EnableKeyRepeat(0, 0);
	    game();
	    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY / 2, SDL_DEFAULT_REPEAT_INTERVAL * 2);
	    jatko = 0;
	    if (a < levels - 1)
		jatko = statusscreen();
	    else
		endgamescreen();
	    if (jatko == 1)
		a = levels;
	}
    }

    return EXIT_SUCCESS;
}
