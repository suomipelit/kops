/* menu.c
 * Copyright 1996,2001,2003 Jetro Lauha.
 *
 * $Id: menu.c,v 1.1 2003/04/15 18:15:49 tonic Exp $
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
#include "logo.h"
#include "spr.h"
#include "load.h"
#include "init.h"
#include "sound.h"

PIC menupic, menublur;
Uint32 menufrm;
Uint8 *menutmp;

void dostatus()
{
    int a, max = 0;

    for (a = 0; a < SDLK_LAST; a++)
    {
	clearkey(a);
    }

    jvcfillbox(0, 0, 639, 479, 0);
    util_loadpcxpi(MENUBACKPICFILE, &menupic);

    for (a = 0; a < 64; a++)
    {
	jpal(a, a, a, a);
    }

    jloadpcxpalpart(KOPSGFXFILE, 128, 191);
    jloadpcxpalpart(KOPSSHIPFILE, 192, 255);
    jsavedest();
    jclearstart();
    jclearpal();

    for (a = 0; a < 480; a++)
    {
	jvcblock(0, 479 - a, 640, 1, menupic.pic + a * 640);
    }

    util_freepi(&menupic);
    kprintfs(bigfont[0], 270, 200, "wins");

    for (a = 0; a < players; a++)
    {
	if (wins[a] > max)
        {
	    max = wins[a];
	}
    }

    if (max == 0)
    {
	max = 1;
    }

    for (a = 0; a < players; a++)
    {
	kprintfs(bigfont[(wins[a] == max) ? 1 : 0], 195, 250 + a * 40, "player %d: %d", a + 1, wins[a]);
	jvcsprite(150, 246 + a * 40, SHIPW, SHIPH, plr[a].ship);
    }

    if (testmode == 1)
    {
	kprintfs(font, 1, 1, "average fps: %d.%d",
		 TIMERRATE * drawnframes / totalframes, (TIMERRATE * 10 * drawnframes / totalframes) % 10);
    }
}

char *lscut(char *instr);

Uint8 statusscreen()
{
    int a;

    sound_stopplay();
    sound_play(m_status);
    sound_pvol(64);
    dostatus();
    kprintfs(bigfont[0], 195, 32, "\334 status \334");
    kprintfs(bigfont[0], 257, 80, "round");
    kprintfs(bigfont[1], 170, 120, "%4d    %d", round_number, levels);
    kprintfs(bigfont[0], 295, 120, "of");
    kprintfs(bigfont[0], 70, 425, "next level: %s", lscut(levlist[round_number]) + 1);
    kprintfs(font, 108, 465, "press enter or space to continue or esc to exit");

    jrealfade(0, 255, 8);
    while ((waskey(K_ENTER) == 0) && (waskey(K_SPACE) == 0) &&
	   (waskey(K_ESC) == 0) && (waskey(K_ENTER2) == 0))
    {
	update();
    };

    jsavestart();
    jcleardest();
    jinitreal();
    for (a = 8; a > 0; a--)
    {
	jvrc();
	sound_pvol(a * 4);
	jrealfade1(0, 255, 7);
    }
    jvcfillbox(0, 0, 639, 479, 0);
    if (waskey(K_ESC))
	return (1);
    return (0);
}

void endgamescreen()
{
    int a;

    sound_stopplay();
    sound_play(m_endgame);
    sound_pvol(64);
    dostatus();
    kprintfs(bigfont[0], 107, 32, "\337 final results \337");
    kprintfs(font, 153, 465, "press enter, space or esc to continue");

    jrealfade(0, 255, 8);
    while ((waskey(K_ENTER) == 0) && (waskey(K_SPACE) == 0) &&
	   (waskey(K_ESC) == 0) && (waskey(K_ENTER2) == 0))
    {
	update();
    };

    jsavestart();
    jcleardest();
    jinitreal();

    for (a = 8; a > 0; a--)
    {
	jvrc();
	sound_pvol(a * 4);
	jrealfade1(0, 255, 7);
    }
    jvcfillbox(0, 0, 639, 479, 0);

}

void info()
{
    int a;

    sound_stopplay();
    sound_play(m_info);
    sound_pvol(64);
    sound_eff(S_MENUCHOOS, 24, 128, 16384, 1);

    for (a = 0; a < SDLK_LAST; a++)
    {
	clearkey(a);
    }

    jvdump(0, 640 * 480, menupic.pic);

    update();

    kprintfs(bigfont[0], 10, 30, "programming, artwork");
    kprintfs(bigfont[0], 12, 60, "and sound effects");
    kprintfs(bigfont[0], 15, 90, "by");
    kprintfs(bigfont[1], 325, 90, "Jetro Lauha");
    kprintfs(bigfont[0], 25, 130, "music by erkki turunen");
    kprintfs(bigfont[0], 15, 160, "A.I by Sami Makinen-Okubo");
    kprintfs(font, 20, 195, "uses SDL (www.libsdl.org)");
    kprintfs(font, 20, 210, "and FMOD (www.fmod.org)");
    kprintfs(font, 20, 225, "and ZLIB (by Jean-loup Gailly and Mark Adler)");
    kprintfs(font, 30, 245, "Additional thanks to Jari Komppa, Pepe Taskinen");
    kprintfs(font, 30, 260, "and Antti Tapaninen");

    kprintfs(bigfont[0], 32, 290, "this is free registered");
    kprintfs(bigfont[0], 32, 320, "version of kops.");
    kprintfs(bigfont[0], 32, 350, "you may copy the");
    kprintfs(bigfont[0], 32, 380, "distribution package");
    kprintfs(bigfont[0], 32, 410, "freely if intact.");

    kprintfs(font, 153, 465, "press enter, space or esc to continue");
    while ((waskey(K_ENTER) == 0) && (waskey(K_SPACE) == 0) &&
	   (waskey(K_ESC) == 0) && (waskey(K_ENTER2) == 0))
    {
	update();
    };
    for (a = 0; a < SDLK_LAST; a++)
    {
	clearkey(a);
    }

    sound_stopplay();
    sound_play(m_startmenu);
    sound_pvol(64);
    sound_eff(S_MENUCHANG, 32, 128, 16384, 1);
}

void gamewindowbackgrounds()
{
    Uint8 *tpic = NULL;
    int a;

    tpic = (Uint8 *) malloc(WINW * WINH + 4);

    if (tpic == NULL)
    {
	jerror("GAMEWINDOWBACKGROUNDS.malloc: No memory", WINW * WINH + 4);
    }

    util_loadpcx((char *) tpic, FILLERPICFILE, WINW * WINH);
    gamewindowcolorrand = (Uint8) (rand() % 2);

    if (gamewindowcolorrand)
    {
	for (a = 0; a < WINW * WINH; a++)
	    *(tpic + a) -= 8;
    }

    jvcblockclip(0, 0, 320, 240, 0, 0, 639, 479, tpic);
    jvcblockclip(320, 0, 320, 240, 0, 0, 639, 479, tpic);
    jvcblockclip(0, 240, 320, 240, 0, 0, 639, 479, tpic);
    jvcblockclip(320, 240, 320, 240, 0, 0, 639, 479, tpic);
    free(tpic);
    tpic = NULL;
}

int lm1win, lm2win, lm1sel, lm2sel, lm3sel, lmsel;
char lscutstr[20];

char *lscut(char *instr)
{
    sscanf(instr, "%[^.]", lscutstr);
    return (lscutstr);
}

void levwin1()
{
    int a, f;

    for (a = 0; a < SDLK_LAST; a++)
    {
	clearkey(a);
    }

    for (a = 205; a < 480; a++)
    {
	memcpy(menutmp + (a - 205) * 225, menupic.pic + a * 640 + 10, 225);
    }

    for (a = 0; a < 11; a++)
    {
	if ((lm1win + a) < alllevels)
        {
	    f = (((lmsel == 1) && (lm1sel == (lm1win + a))) ? 1 : 0);
	    kprintf(menutmp, bigfont[f], 0, a * 25, 225, lscut(levall[lm1win + a]));
	}
    }

    jvcblock(10, 205, 225, 275, menutmp);
}

void levwin2()
{
    int a, f;

    for (a = 0; a < SDLK_LAST; a++)
    {
	clearkey(a);
    }

    for (a = 205; a < 480; a++)
    {
	memcpy(menutmp + (a - 205) * 225, menupic.pic + a * 640 + 245, 225);
    }

    for (a = 0; a < 11; a++)
    {
	if ((lm2win + a) < levels)
        {
	    f = (((lmsel == 2) && (lm2sel == (lm2win + a))) ? 1 : 0);
	    kprintf(menutmp, bigfont[f], 0, a * 25, 225, lscut(levlist[lm2win + a]));
	    if (lm2sel == (lm2win + a))
            {
		util_line(0, a * 25, 224, a * 25, 253, 225, (char *) menutmp);
		util_line(0, a * 25, 0, a * 25 + 23, 253, 225, (char *) menutmp);
		util_line(224, a * 25, 224, a * 25 + 23, 253, 225, (char *) menutmp);
		util_line(0, a * 25 + 23, 224, a * 25 + 23, 253, 225, (char *) menutmp);
	    }
	}
    }

    jvcblock(245, 205, 225, 275, menutmp);
}

void levwin3()
{
#define LEVMENUOPT 4
    char *menustr[LEVMENUOPT] = { "delete", "clear", "random", "exit" };
    int a;

    for (a = 0; a < SDLK_LAST; a++)
    {
	clearkey(a);
    }

    for (a = 0; a < LEVMENUOPT; a++)
    {
	kprintfs(bigfont[0], 480, 220 + a * 30, menustr[a]);
    }

    if (lmsel == 3)
    {
	kprintfs(bigfont[1], 480, 220 + lm3sel * 30, menustr[lm3sel]);
    }

}

void levelsmenu()
{
#define LEVMENUOPT 4
    Uint8 menuexit = 255;
    int a;
    lm1win = 0;
    lm2win = 0;
    lm1sel = 0;
    lm2sel = 0;
    lm3sel = 3;
    lmsel = 3;

    for (a = 0; a < SDLK_LAST; a++)
    {
	clearkey(a);
    }

    jvdump(180 * 640, 300 * 640, menupic.pic + 180 * 640);
    update();

    kprintfs(font, 20, 180, "add levels");
    kprintfs(font, 255, 180, "selected levels");

    jvline(3, 198, 3, 479, 16);
    jvline(476, 199, 476, 479, 10);
    jvline(3, 198, 475, 198, 16);
    jvline(6, 201, 6, 479, 10);
    jvline(6, 201, 237, 201, 10);
    jvline(238, 202, 238, 479, 16);
    jvline(241, 201, 241, 479, 10);
    jvline(241, 201, 472, 201, 10);
    jvline(473, 202, 473, 479, 16);

    levwin1();
    levwin2();
    levwin3();

    while (menuexit == 255)
    {

	wait_framecount();
	menufrm += framecounter;
	framecounter = 0;
	/*  logo */
	logo(menublur.pic, menupic.pic, menufrm);
	jvdump(0, menublur.w * menublur.h, menublur.pic);
	switch (lmsel)
        {
            case 1:
                if ((waskey(K_RIGHT)) || (waskey(K_RIGHT2)))
                {
                    sound_eff(S_MENUCHANG, 48, 128, 16384, 1);
                    lmsel++;
                    levwin1();
                    levwin2();
                    clearkey(K_RIGHT);
                    clearkey(K_RIGHT2);
                }
                if (((waskey(K_UP)) || (waskey(K_UP2))) && (lm1sel > 0))
                {
                    sound_eff(S_MENUMOVE, 64, 128, 16384, 1);
                    lm1sel--;
                    if (lm1win > lm1sel)
                        lm1win--;
                    levwin1();
                    clearkey(K_UP);
                    clearkey(K_UP2);
                }
                if (((waskey(K_DOWN)) || (waskey(K_DOWN2))) && (lm1sel < alllevels - 1))
                {
                    sound_eff(S_MENUMOVE, 64, 128, 16384, 1);
                    lm1sel++;
                    if (lm1win < lm1sel - 10)
                        lm1win++;
                    levwin1();
                    clearkey(K_DOWN);
                    clearkey(K_DOWN2);
                }
                if (((waskey(K_SPACE)) || (waskey(K_ENTER)) || (waskey(K_ENTER2))) && (levels < MAXLEVELS))
                {
                    sound_eff(S_MENUCHOOS, 48, 128, 16384, 1);
                    for (a = levels; a > lm2sel; a--)
                        strcpy(levlist[a], levlist[a - 1]);
                    strcpy(levlist[lm2sel], levall[lm1sel]);
                    levels++;
                    levwin1();
                    levwin2();
                    clearkey(K_SPACE);
                    clearkey(K_ENTER);
                    clearkey(K_ENTER2);
                }
                break;
            case 2:
                if ((waskey(K_LEFT)) || (waskey(K_LEFT2)))
                {
                    sound_eff(S_MENUCHANG, 48, 128, 16384, 1);
                    lmsel--;
                    levwin1();
                    levwin2();
                    clearkey(K_LEFT);
                    clearkey(K_LEFT2);
                }
                if ((waskey(K_RIGHT)) || (waskey(K_RIGHT2)))
                {
                    sound_eff(S_MENUCHANG, 48, 128, 16384, 1);
                    lmsel++;
                    levwin2();
                    levwin3();
                    clearkey(K_RIGHT);
                    clearkey(K_RIGHT2);
                }
                if (((waskey(K_UP)) || (waskey(K_UP2))) && (lm2sel > 0))
                {
                    sound_eff(S_MENUMOVE, 64, 128, 16384, 1);
                    lm2sel--;
                    if (lm2win > lm2sel)
                        lm2win--;
                    levwin2();
                    clearkey(K_UP);
                    clearkey(K_UP2);
                }
                if (((waskey(K_DOWN)) || (waskey(K_DOWN2))) && (lm2sel < levels - 1))
                {
                    sound_eff(S_MENUMOVE, 64, 128, 16384, 1);
                    lm2sel++;
                    if (lm2win < lm2sel - 10)
                        lm2win++;
                    levwin2();
                    clearkey(K_DOWN);
                    clearkey(K_DOWN2);
                }
                break;
            case 3:
                if (((waskey(K_UP)) || (waskey(K_UP2))) && (lm3sel > 0))
                {
                    sound_eff(S_MENUMOVE, 64, 128, 16384, 1);
                    lm3sel--;
                    levwin3();
                    clearkey(K_UP);
                    clearkey(K_UP2);
                }
                if (((waskey(K_DOWN)) || (waskey(K_DOWN2))) && (lm3sel < LEVMENUOPT - 1))
                {
                    sound_eff(S_MENUMOVE, 64, 128, 16384, 1);
                    lm3sel++;
                    levwin3();
                    clearkey(K_DOWN);
                    clearkey(K_DOWN2);
                }
                if ((waskey(K_LEFT)) || (waskey(K_LEFT2)))
                {
                    sound_eff(S_MENUCHANG, 48, 128, 16384, 1);
                    lmsel--;
                    levwin3();
                    levwin2();
                    clearkey(K_LEFT);
                    clearkey(K_LEFT2);
                }
                if ((waskey(K_ENTER)) || (waskey(K_SPACE)) || (waskey(K_ENTER2)))
                {
                    sound_eff(S_MENUCHOOS, 48, 128, 16384, 1);
                    switch (lm3sel)
                    {
                        case 0:
                            if (levels > 1)
                            {
                                for (a = lm2sel; a < levels; a++)
                                    strcpy(levlist[a], levlist[a + 1]);
                                levels--;
                                if (lm2sel >= levels)
                                    lm2sel--;
                                levwin2();
                            }
                            break;
                        case 1:
                            levels = 1;
                            strcpy(levlist[0], levall[rand() % alllevels]);
                            levwin2();
                            break;
                        case 2:
                            for (a = 0; a < levels; a++)
                                strcpy(levlist[a], levall[rand() % alllevels]);
                            levwin2();
                            break;
                        case 3:
                            menuexit = 3;
                            break;
                    }
                    clearkey(K_ENTER);
                    clearkey(K_SPACE);
                    clearkey(K_ENTER2);
                }
                break;
	}
	if (waskey(K_ESC))
        {
        clearkey(K_ESC);
	    menuexit = 3;
	}
	update();
    }
}

void optionsmenu()
{
#define OPTMENUOPT 6
    char *menustr[OPTMENUOPT] = { "parallax <   >", "weapon slots <  >",
                                  "music volume \261\261\261\261\261\261\261\261", "effects vol. \261\261\261\261\261\261\261\261",
                                  "gravity <   >", "exit"
    };
    char *gravtxt[5] = { "no ", "low", "med", "hi ", "*hi" };
    int a, b, sel = 5;
    Uint8 menuexit = 255;

    jvdump(180 * 640, 300 * 640, menupic.pic + 180 * 640);
    update();

    for (a = 0; a < OPTMENUOPT; a++)
    {
	kprintfs(bigfont[0], (640 - strlen(menustr[a]) * 25) / 2, 270 + a * 25, menustr[a]);
    }

    kprintfs(bigfont[0], (640 - strlen(menustr[0]) * 25) / 2 + 250, 270, "%s", parallaxdraw ? "on" : "off");
    kprintfs(bigfont[0], (640 - strlen(menustr[1]) * 25) / 2 + 350, 295, "%d", slots);

    for (a = 0; a < s_musvol / 8; a++)
    {
	kprintfs(bigfont[0], (640 - strlen(menustr[2]) * 25) / 2 + 325 + a * 25, 320, "\333");
    }

    for (a = 0; a < s_effvol / 8; a++)
    {
	kprintfs(bigfont[0], (640 - strlen(menustr[3]) * 25) / 2 + 325 + a * 25, 345, "\333");
    }

    kprintfs(bigfont[0], (640 - strlen(menustr[4]) * 25) / 2 + 225, 370, gravtxt[gravvalue]);

    for (a = 0; a < SDLK_LAST; a++)
    {
	clearkey(a);
    }

    while (menuexit == 255)
    {

	wait_framecount();
	menufrm += framecounter;
	framecounter = 0;

	/*  logo */
	logo(menublur.pic, menupic.pic, menufrm);
	jvdump(0, menublur.w * menublur.h, menublur.pic);

	/*  active row & small rotating ships */
	memcpy(menutmp, menupic.pic + (sel * 25 + 270) * 640, 640 * 25);
	kprintf(menutmp, bigfont[1], (640 - strlen(menustr[sel]) * 25) / 2, 0, 640, menustr[sel]);

	if (sel == 0)
        {
	    kprintf(menutmp, bigfont[1], (640 - strlen(menustr[0]) * 25) / 2 + 250, 0, 640, "%s",
		    parallaxdraw ? "on" : "off");
	}

	if (sel == 1)
        {
	    kprintf(menutmp, bigfont[1], (640 - strlen(menustr[1]) * 25) / 2 + 350, 0, 640, "%d", slots);
	}

	if (sel == 2)
        {
	    for (a = 0; a < s_musvol / 8; a++)
            {
		kprintf(menutmp, bigfont[1], (640 - strlen(menustr[2]) * 25) / 2 + 325 + a * 25, 0, 640, "\333");
	    }
	}

	if (sel == 3)
        {
	    for (a = 0; a < s_effvol / 8; a++)
            {
		kprintf(menutmp, bigfont[1], (640 - strlen(menustr[3]) * 25) / 2 + 325 + a * 25, 0, 640, "\333");
	    }
	}

	if (sel == 4)
        {
	    kprintf(menutmp, bigfont[1], (640 - strlen(menustr[4]) * 25) / 2 + 225, 0, 640,
		    gravtxt[gravvalue]);
	}

	jcscalesprite((640 - strlen(menustr[sel]) * 25) / 2 + strlen(menustr[sel]) * 25 + 16, 4, 32, 32, 16,
		      16, 0, 0, 639, 24, 640, menutmp, othgfx[2].pic + ((menufrm / 2 + 32) & 63) * 32 * 32);

	for (b = 0; b < 32; b++)
        {
	    for (a = 0; a < 32; a++)
            {
		*(menutmp + 640 * 32 + (b << 5) + a) =
		    *(othgfx[2].pic + (((menufrm >> 1) & 63) << 10) + ((31 - b) << 5) + 31 - a);
	    }
	}

	jcscalesprite((640 - strlen(menustr[sel]) * 25) / 2 - 32, 4, 32, 32, 16, 16, 0, 0, 639, 24, 640,
		      menutmp, menutmp + 640 * 32);
	jvdump((270 + sel * 25) * 640, 640 * 25, menutmp);

	/*  keyboard check */
	if ((waskey(K_UP)) || (waskey(K_DOWN)) || (waskey(K_ESC)) || (waskey(K_UP2)) || (waskey(K_DOWN2)))
        {
	    sound_eff(S_MENUMOVE, 64, 128, 16384, 1);
	    memcpy(menutmp, menupic.pic + (sel * 25 + 270) * 640, 640 * 25);
	    kprintf(menutmp, bigfont[0], (640 - strlen(menustr[sel]) * 25) / 2, 0, 640, menustr[sel]);
	    if (sel == 0)
		kprintf(menutmp, bigfont[0], (640 - strlen(menustr[0]) * 25) / 2 + 250, 0, 640, "%s",
			parallaxdraw ? "on" : "off");
	    if (sel == 1)
		kprintf(menutmp, bigfont[0], (640 - strlen(menustr[1]) * 25) / 2 + 350, 0, 640, "%d", slots);
	    if (sel == 2)
		for (a = 0; a < s_musvol / 8; a++)
		    kprintf(menutmp, bigfont[0], (640 - strlen(menustr[2]) * 25) / 2 + 325 + a * 25, 0, 640,
			    "\333");
	    if (sel == 3)
		for (a = 0; a < s_effvol / 8; a++)
		    kprintf(menutmp, bigfont[0], (640 - strlen(menustr[3]) * 25) / 2 + 325 + a * 25, 0, 640,
			    "\333");
	    if (sel == 4)
		kprintf(menutmp, bigfont[0], (640 - strlen(menustr[4]) * 25) / 2 + 225, 0, 640,
			gravtxt[gravvalue]);
	    jvdump((270 + sel * 25) * 640, 640 * 25, menutmp);
	    if (((waskey(K_UP)) || (waskey(K_UP2))) && (sel > 0))
            {
		sel--;
	    }
	    if (((waskey(K_DOWN)) || (waskey(K_DOWN2))) && (sel < OPTMENUOPT - 1))
            {
		sel++;
	    }
	    if (waskey(K_ESC))
            {
		menuexit = 4;
	    }
        clearkey(K_UP);
        clearkey(K_DOWN);
        clearkey(K_ESC);
        clearkey(K_UP2);
        clearkey(K_DOWN2);
	}
	if ((waskey(K_ENTER)) || (waskey(K_SPACE)) || (waskey(K_ENTER2)))
        {
	    sound_eff(S_MENUCHANG, 48, 128, 16384, 1);
	    switch (sel)
            {
                case 0:
                    parallaxdraw = (Uint8) (1 - parallaxdraw);
                    break;
                case 5:
                    menuexit = 5;
                    break;
	    }
        clearkey(K_ENTER);
        clearkey(K_SPACE);
        clearkey(K_ENTER2);
	}
	if ((waskey(K_LEFT)) || (waskey(K_RIGHT)) || (waskey(K_LEFT2)) || (waskey(K_RIGHT2)))
        {
	    sound_eff(S_MENUCHANG, 48, 128, 16384, 1);
	    switch (sel)
            {
                case 0:
                    parallaxdraw = (Uint8) (1 - parallaxdraw);
                    break;
                case 1:
                    if (((waskey(K_LEFT)) || (waskey(K_LEFT2))) && (slots > 2))
                        slots--;
                    if (((waskey(K_RIGHT)) || (waskey(K_RIGHT2))) && (slots < 16))
                        slots++;
                    break;
                case 2:
                    if (((waskey(K_LEFT)) || (waskey(K_LEFT2))) && (s_musvol / 8 > 0))
                        s_musvol = ((s_musvol / 8) - 1) * 8;
                    if (((waskey(K_RIGHT)) || (waskey(K_RIGHT2))) && (s_musvol / 8 < 8))
                        s_musvol = ((s_musvol / 8) + 1) * 8;
                    sound_pvol(64);
                    break;
                case 3:
                    if (((waskey(K_LEFT)) || (waskey(K_LEFT2))) && (s_effvol / 8 > 0))
                        s_effvol = ((s_effvol / 8) - 1) * 8;
                    if (((waskey(K_RIGHT)) || (waskey(K_RIGHT2))) && (s_effvol / 8 < 8))
                        s_effvol = ((s_effvol / 8) + 1) * 8;
                    sound_pvol(64);
                    break;
                case 4:
                    if (((waskey(K_LEFT)) || (waskey(K_LEFT2))) && (gravvalue > 0))
                        gravvalue--;
                    if (((waskey(K_RIGHT)) || (waskey(K_RIGHT2))) && (gravvalue < 3))
                        gravvalue++;
                    initgravity();
                    sound_pvol(64);
                    break;
	    }
        clearkey(K_LEFT);
        clearkey(K_RIGHT);
        clearkey(K_LEFT2);
        clearkey(K_RIGHT2);
	}

	update();
    }

}

Uint8 mainmenu()
{
    enum menu_options {
	MENU_start,
	MENU_players,
	MENU_bots,
	MENU_levels,
	MENU_ignore,
	MENU_options,
	MENU_info,
	MENU_quit,
	MENU_total
    };

    char *menustr[MENU_total] = {
	"start game",
	"players < >",
	"bots < >",
	"choose levels",
	"\372\372",
	"options",
	"info",
	"quit"
    };



    int a, b, sel = 0;
    Uint8 menuexit = 255;
    int fade = 9;

    sound_stopplay();
    sound_play(m_startmenu);
    sound_pvol(64);

    jvcfillbox(0, 0, 639, 479, 0);
    util_loadpcxpi(MENUBACKPICFILE, &menupic);
    util_loadpcxpi(LOGOPICFILE, &menublur);

    memcpy(menublur.pic, menupic.pic, menublur.w * menublur.h);

    logo_init();

    for (a = 0; a < 64; a++)
    {
	jpal(a, a, a, a);
    }

    jloadpcxpalpart(KOPSGFXFILE, 128, 191);
    jpal(253, 40, 30, 20);
    jsavedest();
    jclearstart();
    jclearpal();
    jinitreal();
    kprintf(menupic.pic, font, 430, 469, 640, "free registered version");

    /*  small version number */
    kprintf(menupic.pic, font, 5, 469, 640, "version %d.%d", versmaj, versmin);
    for (a = 455 * 640; a < 480 * 640; a++)
    {
	if (*(menupic.pic + a) > 127)
        {
	    *(menupic.pic + a) = (*(menupic.pic + a) - 136) / 3 + 133;
	}
    }

    jvdump(0, 640 * 480, menupic.pic);

    update();

    for (a = 0; a < MENU_total; a++)
    {
	kprintfs(bigfont[0], (640 - strlen(menustr[a]) * 25) / 2, 220 + a * 32, menustr[a]);
    }

    kprintfs(bigfont[0], (640 - strlen(menustr[MENU_players]) * 25) / 2 + 225, 252, "%d", players);
    kprintfs(bigfont[0], (640 - strlen(menustr[MENU_bots]) * 25) / 2 + 150, 284, "%d", bots);

    menutmp = (Uint8 *) malloc(640 * 300 + 32 * 32);
    menufrm = 0;

    for (a = 0; a < SDLK_LAST; a++)
	clearkey(a);

    while (menuexit == 255)
    {

	wait_framecount();

	menufrm += framecounter;
	framecounter = 0;
	//}

	/*  fadein */
	if (fade > 0)
        {
	    jrealfade1(0, 255, 7);
	    fade--;
	}

	/*  logo */
	logo(menublur.pic, menupic.pic, menufrm);
	jvdump(0, menublur.w * menublur.h, menublur.pic);

	/*  active row & rotating ships */
	memcpy(menutmp, menupic.pic + (sel * 32 + 216) * 640, 640 * 32);
	kprintf(menutmp, bigfont[1], (640 - strlen(menustr[sel]) * 25) / 2, 4, 640, menustr[sel]);

	if (sel == MENU_players)
        {
	    kprintf(menutmp, bigfont[1], (640 - strlen(menustr[MENU_players]) * 25) / 2 + 225,
		    4, 640, "%d", players);
	}
        else if ( sel == MENU_bots )
        {
	    kprintf(menutmp, bigfont[1], (640 - strlen(menustr[MENU_bots]) * 25) / 2 + 150,
		    4, 640, "%d", bots);
	}

	jcsprite((640 - strlen(menustr[sel]) * 25) / 2 - 48, 0,
		 32, 32, 640, menutmp, othgfx[2].pic + ((menufrm / 2 + 32) & 63) * 32 * 32);
	jcscalesprite((640 - strlen(menustr[sel]) * 25) / 2 + strlen(menustr[sel]) * 25 + 48, 8,
		      32, 32, 16, 16, 0, 0, 639, 31, 640, menutmp,
		      othgfx[2].pic + ((menufrm / 2 + 32) & 63) * 32 * 32);

	for (b = 0; b < 32; b++)
        {
	    for (a = 0; a < 32; a++)
            {
		*(menutmp + 640 * 32 + (b << 5) + a) =
		    *(othgfx[2].pic + (((menufrm >> 1) & 63) << 10) + ((31 - b) << 5) + 31 - a);
	    }
	}

	jcsprite((640 - strlen(menustr[sel]) * 25) / 2 + strlen(menustr[sel]) * 25 + 16, 0, 32, 32, 640,
		 menutmp, menutmp + 640 * 32);
	jcscalesprite((640 - strlen(menustr[sel]) * 25) / 2 - 64, 8, 32, 32, 16, 16, 0, 0, 639, 31, 640,
		      menutmp, menutmp + 640 * 32);
	jvdump((216 + sel * 32) * 640, 640 * 32, menutmp);

	/*  keyboard check */
	if ((waskey(K_UP)) || (waskey(K_DOWN)) || (waskey(K_ESC)) || (waskey(K_UP2)) || (waskey(K_DOWN2)))
        {

	    sound_eff(S_MENUMOVE, 64, 128, 16384, 0);

	    memcpy(menutmp, menupic.pic + (sel * 32 + 216) * 640, 640 * 32);
	    kprintf(menutmp, bigfont[0], (640 - strlen(menustr[sel]) * 25) / 2, 4, 640, menustr[sel]);

	    if (sel == MENU_players)
            {
		kprintf(menutmp, bigfont[0], (640 - strlen(menustr[MENU_players]) * 25) / 2 + 225, 4, 640, "%d",
			players);
	    }
	    else if (sel == MENU_bots)
            {
		kprintf(menutmp, bigfont[0], (640 - strlen(menustr[MENU_bots]) * 25) / 2 + 150, 4, 640, "%d",
			bots);
	    }

	    jvdump((216 + sel * 32) * 640, 640 * 32, menutmp);

	    if (((waskey(K_UP)) || (waskey(K_UP2))) && (sel > 0))
            {
		sel--;
		if (sel == MENU_ignore )
                {
		    sel--;
		}
	    }
	    if (((waskey(K_DOWN)) || (waskey(K_DOWN2))) && (sel < MENU_total - 1))
            {
		sel++;
		if (sel == MENU_ignore )
                {
		    sel++;
		}
	    }
	    if (waskey(K_ESC))
            {
		if (sel == MENU_quit)
                {
		    menuexit = 6;
		}
		sel = MENU_quit;
	    }

        clearkey(K_UP);
        clearkey(K_DOWN);
        clearkey(K_ESC);
        clearkey(K_UP2);
        clearkey(K_DOWN2);
	}

	if ((waskey(K_LEFT)) || (waskey(K_RIGHT)) || (waskey(K_LEFT2)) || (waskey(K_RIGHT2)))
        {

	    sound_eff(S_MENUCHANG, 48, 128, 16384, 1);

	    if (sel == MENU_players)
            {
		if (((waskey(K_LEFT)) || (waskey(K_LEFT2))) && (players > 1))
                {
		    deinitplayer(players - 1);
		    players--;
		}
		if (((waskey(K_RIGHT)) || (waskey(K_RIGHT2))) && (players < MAXPLAYERS))
                {
		    initplayer(players);
		    players++;
		}
	    }
	    else if (sel == MENU_bots)
            {
		if (((waskey(K_LEFT)) || (waskey(K_LEFT2))) && (bots > 0))
                {
		    deinitplayer(MAXPLAYERS + bots - 1);
		    --bots;
		}
		if (((waskey(K_RIGHT)) || (waskey(K_RIGHT2))) && (bots < MAXBOTS))
                {
		    initplayer(MAXPLAYERS + bots);
		    ++bots;
		}
	    }
        clearkey(K_LEFT);
        clearkey(K_RIGHT);
        clearkey(K_LEFT2);
        clearkey(K_RIGHT2);
	}
	if (((waskey(K_ENTER)) || (waskey(K_SPACE)) || (waskey(K_ENTER2))) && (fade <= 0))
        {
	    sound_eff(S_MENUCHOOS, 48, 128, 16384, 1);
	    switch (sel)
            {
                case MENU_start:
                    menuexit = 0;
                    break;
                case MENU_quit:
                    menuexit = 6;
                    break;
                case MENU_levels:
                    levelsmenu();
                    jvdump(180 * 640, 300 * 640, menupic.pic + 180 * 640);
                    for (a = 0; a < MENU_total; a++)
                    {
                        kprintfs(bigfont[0], (640 - strlen(menustr[a]) * 25) / 2, 220 + a * 32, menustr[a]);
                    }
                    kprintfs(bigfont[0], (640 - strlen(menustr[MENU_players]) * 25) / 2 + 225, 252, "%d", players);
                    kprintfs(bigfont[0], (640 - strlen(menustr[MENU_bots]) * 25) / 2 + 150, 284, "%d", bots);
                    break;
                case MENU_options:
                    optionsmenu();
                    jvdump(180 * 640, 300 * 640, menupic.pic + 180 * 640);
                    for (a = 0; a < MENU_total; a++)
                    {
                        kprintfs(bigfont[0], (640 - strlen(menustr[a]) * 25) / 2, 220 + a * 32, menustr[a]);
                    }
                    kprintfs(bigfont[0], (640 - strlen(menustr[MENU_players]) * 25) / 2 + 225, 252, "%d", players);
                    kprintfs(bigfont[0], (640 - strlen(menustr[MENU_bots]) * 25) / 2 + 150, 284, "%d", bots);
                    break;
                case MENU_info:
                    info();
                    jvdump(180 * 640, 300 * 640, menupic.pic + 180 * 640);
                    for (a = 0; a < MENU_total; a++)
                    {
                        kprintfs(bigfont[0], (640 - strlen(menustr[a]) * 25) / 2, 220 + a * 32, menustr[a]);
                    }
                    kprintfs(bigfont[0], (640 - strlen(menustr[1]) * 25) / 2 + 225, 252, "%d", players);
                    kprintfs(bigfont[0], (640 - strlen(menustr[MENU_bots]) * 25) / 2 + 150, 284, "%d", bots);
                    break;
	    }
        clearkey(K_ENTER);
        clearkey(K_SPACE);
        clearkey(K_ENTER2);
	}

	update();
    }

    free(menutmp);
    menutmp = NULL;

    clearkey(K_ESC);
    logo_deinit();
    util_freepi(&menublur);
    util_freepi(&menupic);
    jsavestart();
    jcleardest();
    jinitreal();

    for (a = 8; a > 0; a--)
    {
	jvrc();
	sound_pvol(a * 4);
	jrealfade1(0, 255, 7);
    }

    jvcfillbox(0, 0, 639, 479, 0);
    update();

    saveconfig();

    return (menuexit);
}
