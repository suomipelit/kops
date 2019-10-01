/* game.c
 * Copyright 1996,2001,2003 Jetro Lauha.
 *
 * $Id: game.c,v 1.1 2003/04/15 18:15:49 tonic Exp $
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
#include "draw.h"
#include "gamefunc.h"
#include "spr.h"
#include "init.h"
#include "sound.h"
#include "kops-bot/reactive-agent/cannonfodder.h"
#include "kops-bot/reactive-agent/bot-util.h"

/* #define VRC */
/* #define RASTER */

#define SCREENSHOTS

int isqrt(int num)
{
    return (int) sqrt((double) num);
}


void preparegame()
{
    int a, b, c;

    for (a = 0; a < MAXCRAP; a++) {
        crap[a].active = 0;
    }

    for (a = 0; a < MAXAMMO; a++) {
        ammo[a].active = 0;
    }

    for (a = 0; a < MAXBOOMS; a++) {
        boom[a].active = 0;
    }

    for (a = 0; a < pickammospots; a++) {
        pickammo[a].active = 0;
    }

    nextammo = 0;
    nextcrap = 0;
    nextboom = 0;

    for (a = 0; a < (MAXPLAYERS + MAXBOTS); a++)
    {
	if (plr[a].active)
        {
	    placeplayer((Uint8) a);
	    plr[a].xi         = plr[a].yi = plr[a].dir = 0;
	    plr[a].bot        = 0;
	    plr[a].firepause  = 0;
	    plr[a].fireflag   = (Uint8) 0;
	    plr[a].blink      = plr[a].ground = plr[a].water = 0;
	    plr[a].activeslot = (Uint8) (plr[a].activeslot % slots);
	    plr[a].nextslot   = (Uint8) (plr[a].nextslot % slots);
	    plr[a].energy     = STARTENERGY << FIXP;
	}
    }

    /* Detach all current agents. */
    detachAgents();

    /* Attach as many agents as required. */
    for(a = MAXPLAYERS; a < (MAXPLAYERS + bots); ++a)
    {
	if( plr[a].active )
        {
	    attachAgent(&plr[a]);
	    plr[a].bot       = 1;
	    plr[a].action    = ACTION_NOT_DEFINED;
	    plr[a].fire_rate = 0;
	}
    }

    for (a = 0; a < SDLK_LAST; a++) {
        clearkey(a);
    }

    basepixcount = 0;
    cnt = 0;

    for (a = 0; a < pickammospots / 10; a++)
    {
	newpickammo();
    }

    for (a = 0; a < slots * 2 / 3; a++)
    {

	for (b = 0; b < players; b++)
        {

	    c = rand() % slots;

	    plr[b].slots[c] = W_NOWEAPON;
	    plr[b].bullets[c] = 0;
	    plr[b].ammo2 = plr[b].slots[plr[b].activeslot];
	    plr[b].ammo1 /= 2;

	    if (plr[b].ammo1 < W_SLING)
            {
		plr[b].ammo1 = W_SLING;
	    }
	}

	/* Same for bots. */
	for (b = MAXPLAYERS; b < (MAXPLAYERS + bots); b++)
        {

	    c = rand() % slots;

	    plr[b].slots[c] = W_NOWEAPON;
	    plr[b].bullets[c] = 0;
	    plr[b].ammo2 = plr[b].slots[plr[b].activeslot];
	    plr[b].ammo1 /= 2;

	    if (plr[b].ammo1 < W_SLING)
            {
		plr[b].ammo1 = W_SLING;
	    }
	}
    }

    if (snd)
    {
	/* sound_stopplay(); */
	sound_play(m_game[rand() % mgcount]);
	sound_pvol(64);
    }

    totalframes = 0;
    drawnframes = 0;

}

void game()
{
    int a, b, c = 0, p, oxi, oyi, ox, oy, oxp, oyp, oenergy[MAXPLAYERS + MAXBOTS];
    /* int tx,ty; */
    char               borderhit;
    unsigned long int  frm = 0;
    int                fade = 20;
    char               resurrect[MAXPLAYERS + MAXBOTS];
    int                pan;
    int                dead = 0, victory = 0;

    for (a = 0; a < (MAXPLAYERS + MAXBOTS); a++) {
        if (plr[a].active) {
            resurrect[a] = 1;
        } else {
            resurrect[a] = 0;
        }
    }

    preparegame();

    while ((!key(K_ESC)) && (!waskey(K_ESC)) && (victory == 0) && (dead < players))
    {

	wait_framecount();

	frm = framecounter;
	framecounter = 0;

	while (frm > 0)
        {
	    cnt++;
	    totalframes++;

	    pan = 128;

	    botHandler();

	    /*
	     * main loop for all players
	     */
	    for (p = 0; p < ( MAXPLAYERS + MAXBOTS ); p++) {
                if ((plr[p].active) && (plr[p].energy > 0))
                {

                    if (players == 1 || plr[p].bot )
                    {
                        pan = 128;
                    }
                    else {
                        if( (p & 1) == 1 )
                        {
                            pan = 32;  /* Even plrs */
                        }
                        else {
                            pan = 224; /* Odd plrs */
                        }
                    }
#if 0
                    switch (p)
                    {
			case 0:
                            pan = 224;
                            break;
			case 1:
                            pan = 32;
                            break;
			case 2:
                            pan = 224;
                            break;
			case 3:
                            pan = 32;
                            break;
			default:
                            /* Bot. */
                            pan = 128;
                            break;
                    }
                }
#endif

            /* sound_eff(S_MENUCHANG,64,128,16384,1); */

            oenergy[p] = plr[p].energy;
            oxi        = plr[p].xi;
            oyi        = plr[p].yi;
            ox         = plr[p].x;
            oy         = plr[p].y;
            oxp        = plr[p].xp;
            oyp        = plr[p].yp;

            /*
             * movement key checks
             */
            if ((plr[p].bot && IS_SET(ACTION_THRUST, plr[p].action)) ||
                (!plr[p].bot && ( (key(plr[p].k_up)) || (waskey(plr[p].k_up)))))
            {
                plr[p].xi += cost[((plr[p].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] / ACCELDIV;
                plr[p].yi += sint[((plr[p].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT] / ACCELDIV;
                newplrcrap((Uint8) p, (Uint8) 0);

                if( ! plr[p].bot )
                {
                    clearkey(plr[p].k_up);
                }

                if (basedetect( plr[p].xp, plr[p].yp ) == 1)
                {
                    plr[p].y -= FIXS;
                }

                if (basedetect( plr[p].xp, plr[p].yp ) == 2)
                {
                    plr[p].y += FIXS;
                }
            }

            if ((basedetect(plr[p].xp, plr[p].yp) != 1) || (plr[p].hyperspac))
            {
                if ((plr[p].bot && IS_SET(ACTION_TURN_RIGHT, plr[p].action)) ||
                    (!plr[p].bot && ((key(plr[p].k_right)) || (waskey(plr[p].k_right)))) )
                {
                    plr[p].dir = (plr[p].dir + TURNRATE) % (SHIPFRM << FIXP);
                    /*logDebug(0, "dir = %f", (double) (plr[p].dir/FIXS));*/

                    if( ! plr[p].bot )
                    {
                        clearkey(plr[p].k_right);
                    }
                }
                if ((plr[p].bot && IS_SET(ACTION_TURN_LEFT, plr[p].action)) ||
                    (!plr[p].bot && ((key(plr[p].k_left))  || (waskey(plr[p].k_left)))))
                {
                    plr[p].dir = (plr[p].dir + ((SHIPFRM << FIXP) - TURNRATE)) % (SHIPFRM << FIXP);

                    if( ! plr[p].bot )
                    {
                        clearkey(plr[p].k_left);
                    }
                }
            }

            /*
             * firing
             */
            if (plr[p].fireflag &&
                ((plr[p].bot && --plr[p].fire_rate < 0) || /* && IS_CLR(ACTION_FIRE_MAIN, plr[p].action) */
                 (!plr[p].bot && !key(plr[p].k_fire1))) )
            {
                plr[p].fireflag  = 0;
                plr[p].fire_rate = 7;
            }

            if (plr[p].firepause > 0)
            {
                --plr[p].firepause;
            }

            if ((plr[p].fireflag == 0) &&
                ((plr[p].bot && IS_SET(ACTION_FIRE_MAIN, plr[p].action)) ||
                 (! plr[p].bot && (key(plr[p].k_fire1) || waskey(plr[p].k_fire1)))))
            {

                sound_eff(plr[p].ammo1, 64, pan, 16384, 0);
                newammo(p, plr[p].ammo1);
                plr[p].fireflag = 1;

                if( ! plr[p].bot )
                {
                    clearkey(plr[p].k_fire1);
                }
            }

            if ((plr[p].firepause > 0) &&
                ((plr[p].bot && IS_SET(ACTION_FIRE_SPECIAL, plr[p].action)) ||
                 (!plr[p].bot && (key(plr[p].k_fire2) || waskey(plr[p].k_fire2)))) )
            {
                if( ! plr[p].bot )
                {
                    clearkey(plr[p].k_fire2);
                }
            }

            if (((plr[p].bot && IS_SET(ACTION_FIRE_SPECIAL, plr[p].action)) ||
                 (!plr[p].bot && (key(plr[p].k_fire2) || waskey(plr[p].k_fire2))))
                && (plr[p].firepause == 0)
                && (plr[p].ammo2 != W_NOWEAPON) )
            {
                newammo(p, plr[p].ammo2);
                plr[p].firepause += weapons[plr[p].ammo2].firepause;
                sound_eff(weapons[plr[p].ammo2].sampleno,
                          weapons[plr[p].ammo2].volume,
                          pan,
                          weapons[plr[p].ammo2].freq,
                          weapons[plr[p].ammo2].chgroup);
                plr[p].xi -= (int)(cost[((plr[p].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT]
                                   * weapons[plr[p].ammo2].recoil);
                plr[p].yi -= (int)(sint[((plr[p].dir >> FIXP) * FRFIX + SCT * 3 / 4) % SCT]
                                   * weapons[plr[p].ammo2].recoil);

                if(weapons[plr[p].ammo2].special != NULL)
                {
                    weapons[plr[p].ammo2].special(p);
                }

                plr[p].bullets[plr[p].activeslot]--;
                if (plr[p].bullets[plr[p].activeslot] <= 0)
                {
                    plr[p].slots[plr[p].activeslot] = W_NOWEAPON;
                    plr[p].ammo2 = W_NOWEAPON;
                }
            }

            if( ! plr[p].bot )
            {
                clearkey(plr[p].k_fire2);
            }

            /*
             * ship hit -- disabled
             */
            /*
             * for (a=0;a<MAXPLAYERS;a++)
             { if
             * ((plr[a].active)&&(plr[a].energy>0)&&(p!=a))
             {
             * tx=plr[p].xp-plr[a].xp; ty=plr[p].yp-plr[a].yp; if
             * ((tx*tx+ty*ty)<(SHIPHITDIST*SHIPHITDIST))
             {
             * plr[p].xi=-plr[p].xi; plr[p].yi=-plr[p].yi;
             * plr[a].xi=-plr[a].xi; plr[a].yi=-plr[a].yi; } } }
             */

            /*
             * gravity etc
             */
            /*if (!basedetect(p))*/

            if (!basedetect(plr[p].xp, plr[p].yp))
            {
                plr[p].yi += gravity;
            }

            /*if (waterdetect(p))
              { */
            if (waterdetect(plr[p].xp, plr[p].yp))
            {
                plr[p].yi += watergrav;
                if (plr[p].water == 0)
                {
                    plr[p].water = 1;
                    newplrcrap(p, 1);
                }
            }
            else {
                plr[p].water = 0;
            }

            if (plr[p].xi > MAXSPEED)
            {
                plr[p].xi = oxi;
            }
            if (plr[p].xi < -MAXSPEED)
            {
                plr[p].xi = oxi;
            }

            if (plr[p].yi > MAXSPEED)
            {
                plr[p].yi = oyi;
            }

            if (plr[p].yi < -MAXSPEED)
            {
                plr[p].yi = oyi;
            }


            plr[p].x += plr[p].xi;
            plr[p].y += plr[p].yi;
            plr[p].xi = plr[p].xi * FRICTION;
            plr[p].yi = plr[p].yi * FRICTION;
            if (plr[p].sensorjam > 0) {
                plr[p].sensorjam--;
            }
            if (plr[p].flip > 0) {
                plr[p].flip--;
            }
            if (plr[p].signal > 0) {
                plr[p].signal--;
            }

            /*
             * border hit detect
             */
            borderhit = 0;
            if (plr[p].x < -12 << FIXP)
            {
                plr[p].x = -12 << FIXP;
                borderhit = 1;
            }
            if (plr[p].x > (levw - SHIPW + 12) << FIXP)
            {
                plr[p].x = (levw - SHIPW + 12) << FIXP;
                borderhit = 1;
            }
            if (plr[p].y < -12 << FIXP)
            {
                plr[p].y = -12 << FIXP;
                borderhit = 1;
            }
            if (plr[p].y > (levh - SHIPH + 12) << FIXP)
            {
                plr[p].y = (levh - SHIPH + 12) << FIXP;
                borderhit = 1;
            }
            if (borderhit)
            {
                plr[p].xi = -plr[p].xi / BOUNCEDIV;
                plr[p].yi = -plr[p].yi / BOUNCEDIV;
                if (plr[p].ground == 0)
                {
                    sound_eff(S_GNDHIT, 64, pan, 16384, 0);
                    plr[p].blink = 1;
                    plr[p].energy -=
                        (isqrt
                         ((plr[p].xi >> (FIXP / 2)) *
                          (plr[p].xi >> (FIXP / 2)) +
                          (plr[p].yi >> (FIXP / 2)) *
                          (plr[p].yi >> (FIXP / 2))) << (FIXP / 2)) * BORHIT;
                    if (plr[p].energy < 0) {
                        plr[p].energy = 0;
                    }
                }
                plr[p].ground = 1;
            }

            /*
             * smoke etc when damaged
             */
            if ((plr[p].energy < (20 << FIXP)) && (cnt % ((plr[p].energy >> (FIXP + 1)) + 1) == 0))
            {
                if ((rand() % 10) == 0) {
                    sound_eff(S_GRAVFLOW, 64, pan, 8192 + (rand() % 8) * 1024, 0);
                }
                newplrcrap(p, 4);
            }
            plr[p].xp = plr[p].x >> FIXP;
            plr[p].yp = plr[p].y >> FIXP;

            /*
             * ground hitdetect
             */

            if (hitdetect(plr[p].xp, plr[p].yp) && (plr[p].hyperspac == 0))
            {
                plr[p].x = ox;
                plr[p].y = oy;
                plr[p].xi = -plr[p].xi / BOUNCEDIV;
                plr[p].yi = -plr[p].yi / BOUNCEDIV;
                plr[p].xp = plr[p].x >> FIXP;
                plr[p].yp = plr[p].y >> FIXP;

                if ((plr[p].ground == 0) && (!basedetect(plr[p].xp, plr[p].yp)))
                {
                    sound_eff(S_GNDHIT, 64, pan, 16384, 0);
                    plr[p].blink = 1;
                    plr[p].energy -=
                        (isqrt
                         ((plr[p].xi >> (FIXP / 2)) *
                          (plr[p].xi >> (FIXP / 2)) +
                          (plr[p].yi >> (FIXP / 2)) *
                          (plr[p].yi >> (FIXP / 2))) << (FIXP / 2)) * GNDHIT;
                    if (plr[p].energy < 0) {
                        plr[p].energy = 0;
                    }
                }
                plr[p].ground = 1;
            }
            else if (borderhit == 0)
            {
                plr[p].ground = 0;
            }

            /*
             * chargebase detect and weapon change
             */
            if (plr[p].slotchangepause > 0)
            {
                --plr[p].slotchangepause;
            }

            a = basedetect(plr[p].xp, plr[p].yp);
            if ((a) && (plr[p].hyperspac == 0))
            {
                plr[p].xi = plr[p].yi = 0;
                if ((dead == (players+bots - 1)) && (players+bots > 1))
                {
                    victory = p + 1;
                }

                if (a == 1)
                {
                    if ((plr[p].dir >> FIXP) < 32)
                    {
                        plr[p].dir = (plr[p].dir + ((SHIPFRM << FIXP) - FIXS)) % (SHIPFRM << FIXP);
                    }
                    else {
                        plr[p].dir = (plr[p].dir + FIXS) % (SHIPFRM << FIXP);
                    }
                    /*
                     * weapon change
                     */
                    if (((!key(plr[p].k_left))
                         && !plr[p].bot
                         && (waskey(plr[p].k_left)))
                        && (plr[p].slotchangepause == 0))
                    {
                        sound_eff(S_TUHNU, 32, pan, 8192, 0);
                        plr[p].activeslot = (Uint8) ((plr[p].activeslot + slots - 1) % slots);
                        plr[p].ammo2 = plr[p].slots[plr[p].activeslot];
                        plr[p].slotchangepause += 15;
                        clearkey(plr[p].k_left);
                    }
                    if (((!key(plr[p].k_right))
                         && !plr[p].bot
                         && (waskey(plr[p].k_right)))
                        && (plr[p].slotchangepause == 0))
                    {
                        sound_eff(S_TUHNU, 32, pan, 8192, 0);
                        plr[p].activeslot = (Uint8) ((plr[p].activeslot + 1) % slots);
                        plr[p].ammo2 = plr[p].slots[plr[p].activeslot];
                        plr[p].slotchangepause += 15;
                        clearkey(plr[p].k_right);
                    }
                    if (((!key(plr[p].k_right))
                         && !plr[p].bot
                         && (!key(plr[p].k_left)))
                        && ((!waskey(plr[p].k_right))
                            && (!waskey(plr[p].k_left)))) {
                        plr[p].slotchangepause = 0;
                    }
                }
                else {	/*
                         * a==2
                         */
                    if ((cnt & 3) == 0) {
                        plr[p].dir = (plr[p].dir + ((SHIPFRM << FIXP) - FIXS)) % (SHIPFRM << FIXP);
                    }
                    if ((cnt & 3) == 2) {
                        plr[p].dir = (plr[p].dir + FIXS) % (SHIPFRM << FIXP);
                    }
                }
                if (plr[p].energy < STARTENERGY << FIXP) {
                    plr[p].energy += CHARGERATE;
                }
                if (plr[p].energy > STARTENERGY << FIXP) {
                    plr[p].energy = STARTENERGY << FIXP;
                }
            }

            /*
             * window coordinates
             */
            plr[p].wx = plr[p].xp + SHIPW / 2 - WINW / 2;
            plr[p].wy = plr[p].yp + SHIPH / 2 - WINH / 2;
            if (plr[p].wx <= 0) {
                plr[p].wx = 0;
            }
            if (plr[p].wx > levw - WINW) {
                plr[p].wx = levw - WINW;
            }
            if (plr[p].wy <= 0) {
                plr[p].wy = 0;
            }
            if (plr[p].wy > levh - WINH) {
                plr[p].wy = levh - WINH;
            }

        }
}
        /*
         * end of main loop
         */

        /*
         * ammos
         */
        for (p = 0; p < MAXAMMO; p++) {
            if (ammo[p].active)
            {
                ammo[p].oxp = ammo[p].xp;
                ammo[p].oyp = ammo[p].yp;

                if(weapons[ammo[p].type].move != NULL) {
                    weapons[ammo[p].type].move(p);
                }

                ammo[p].xp = ammo[p].x >> FIXP;
                ammo[p].yp = ammo[p].y >> FIXP;

                /*
                 * ammo playerhitdetect
                 */
                for (a = 0; a < (MAXPLAYERS + MAXBOTS); a++) {
                    if ((plr[a].active) && (plr[a].energy > 0))
                    {
                        ox = ammo[p].xp - (plr[a].xp + SHIPW / 2);
                        oy = ammo[p].yp - (plr[a].yp + SHIPH / 2);
                        if ((ox < ammo[p].hitdist * 2)
                            && (oy < ammo[p].hitdist * 2))
                        {
                            if ((ox * ox + oy * oy) <= (ammo[p].hitdist * ammo[p].hitdist))
                            {
                                sound_eff(S_SHIPHIT, 64, 128, 16384, 0);
                                plr[a].blink = 1;
                                plr[a].energy -= ammo[p].damage;

                                if (plr[a].energy < 0)
                                {
                                    plr[a].energy = 0;
                                }

                                if(weapons[ammo[p].type].hit != NULL)
                                {
                                    weapons[ammo[p].type].hit(a, p);
                                }
                            }
                        }
                    }
                }

                /*
                 * ammo hitdetect
                 */
                ox = ammo[p].x;
                oy = ammo[p].y;
                if (ammo[p].oyp >= 0 &&
                    ammo[p].yp >= 0 &&
                    ammo[p].oxp >= 0 &&
                    ammo[p].xp >= 0 &&
                    ammo[p].oyp < levh &&
                    ammo[p].yp < levh &&
                    ammo[p].oxp < levw &&
                    ammo[p].xp < levw &&
                    *(mask + ((ammo[p].oyp + ammo[p].yp) / 2) * levw +
                      (ammo[p].oxp + ammo[p].xp) / 2) == 0)
                {
                    ammo[p].xp = (ammo[p].oxp + ammo[p].xp) / 2;
                    ammo[p].yp = (ammo[p].oyp + ammo[p].yp) / 2;
                    ammo[p].x = ammo[p].xp << FIXP;
                    ammo[p].y = ammo[p].yp << FIXP;
                }
                if ((ammo[p].oyp >= 0 &&
                     ammo[p].yp >= 0 &&
                     ammo[p].oxp >= 0 &&
                     ammo[p].xp >= 0 &&
                     ammo[p].oyp < levh &&
                     ammo[p].yp < levh &&
                     ammo[p].oxp < levw &&
                     ammo[p].xp < levw && (*(mask + (ammo[p].yp) * levw + ammo[p].xp) == 0))
                    || (ammo[p].xp < 0) || (ammo[p].yp < 0)
                    || (ammo[p].xp >= levw) || (ammo[p].yp >= levh))
                {
                    /* *(level+(ammo[p].yp)*levw+ammo[p].xp)=0; */
                    /* *(mask+(ammo[p].yp)*levw+ammo[p].xp)=0xff; */
                    if(weapons[ammo[p].type].ammohit != NULL)
                    {
			weapons[ammo[p].type].ammohit(p, ox, oy);
                    }
                }
                if (ammo[p].timeleft > 0) {
                    ammo[p].timeleft--;
                }
                if (ammo[p].timeleft <= 0) {
                    ammo[p].active = 0;
                }
            }
        }

        /*
         * pickammo
         */
        for (p = 0; p < pickammospots; p++) {
            if (pickammo[p].active)
            {
                pickammo[p].cnt = (pickammo[p].cnt + 8) % SCT;
                pickammo[p].y = pickammo[p].orgy + (sint[pickammo[p].cnt] << 3);
                pickammo[p].xp = (pickammo[p].x >> FIXP);
                pickammo[p].yp = (pickammo[p].y >> FIXP);
                /*
                 * pickammo collecting
                 */
                for (a = 0; a < (MAXPLAYERS + MAXBOTS); a++) {
                    if ((plr[a].active) && (plr[a].energy > 0) && (plr[a].hyperspac == 0))
                    {
                        ox = pickammo[p].xp - (plr[a].xp + SHIPW / 2);
                        oy = pickammo[p].yp - (plr[a].yp + SHIPH / 2);
                        if ((ox < PICKDIST * 2) && (oy < PICKDIST * 2))
                        {
                            if ((ox * ox + oy * oy) <= (PICKDIST * PICKDIST))
                            {
                                sound_eff(S_COLLECT, 64, 128, 16384, 0);
                                weapons[pickammo[p].type].pick(a, p);
#if 0
                                switch (pickammo[p].type)
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
					pickammo[p].active = 0;
					plr[a].ammo1 = pickammo[p].type;
					break;
				    case W_GASBLAST:
				    case W_MEGABLAST:
				    case W_MINE:
				    case W_HEATSEEK:
				    case W_MORNSTAR:
				    case W_SENSORJAM:
				    case W_NOWEAPON:
				    case W_PYROTECH:
				    case W_TOXICWAST:
				    case W_TOASTER:
				    case W_REARSLING:
				    case W_RINGFIRE:
				    case W_CRAWLER:
				    case W_GRAVFLOW:
				    case W_BOMB:
				    case W_LASH:
				    case W_ULTRATHR:
				    case W_RETRO:
				    case W_HYPERSPAC:
				    case W_PLASMA:
				    case W_PULSLASER:
				    case W_DELBLAST:
				    case W_FLIPPER:
				    case W_SIGNAL:
				    case W_SPILLER:
#ifdef REG
				    case W_TUHNRUISK:
				    case W_HYPERBLST:
				    case W_TRACTBEAM:
				    case W_TWINLASER:
				    case W_SUPERPYRO:
#endif
					pickammo[p].active = 0;
					if (plr[a].activeslot == plr[a].nextslot)
					    plr[a].nextslot = (Uint8) ((plr[a].nextslot + 1) % slots);
					plr[a].bullets[plr[a].nextslot] = pickammo[p].bullets;
					plr[a].slots[plr[a].nextslot] = pickammo[p].type;
					plr[a].nextslot = (Uint8) ((plr[a].nextslot + 1)
								   % slots);
					break;
				    case W_NEXTAMMO:
					pickammo[p].active = 0;
					plr[a].activeslot = (Uint8) ((plr[a].activeslot + 1) % slots);
					plr[a].ammo2 = plr[a].slots[plr[a].activeslot];
					break;
                                }
#endif
                            }
                        }
                    }
                }
                if (pickammo[p].timeleft > 0) {
                    pickammo[p].timeleft--;
                }
                if (pickammo[p].timeleft <= 0) {
                    pickammo[p].active = 0;
                }
            }
        }
        /*
         * create new pickammo
         */
        if (cnt % PICKRATE == 0)
        {
            newpickammo();
        }

        /*
         * crap
         */
        ox = rand() % levw;
        oy = rand() % levh;
        /*
         * random bubbles
         */
        if (*(level + oy * levw + ox) == 1) {
            newcrap(ox, oy, 3);
        }
        for (p = 0; p < MAXCRAP; p++) {
            if (crap[p].active)
            {	/*
                 * handle crap
                 */
                crap[p].x += crap[p].xi;
                crap[p].y += crap[p].yi;
                crap[p].xp = crap[p].x >> FIXP;
                crap[p].yp = crap[p].y >> FIXP;
                /*
                 * crap hitdetect etc
                 */
                switch (crap[p].type)
                {
		    case 0:
		    case 1:
		    case 2:
		    case 4:
			if ((crap[p].xp < 0) || (crap[p].yp < 0) ||
			    (crap[p].xp >= levw) || (crap[p].yp >= levh) ||
			    (*(mask + (crap[p].yp) * levw + crap[p].xp) == 0)) {
			    crap[p].active = 0;
            }
			break;
		    case 3:
			crap[p].yi -= FIXS / 64;
			if ((crap[p].xp < 0) || (crap[p].yp < 0) ||
			    (crap[p].xp >= levw) || (crap[p].yp >= levh) ||
			    (*(mask + (crap[p].yp) * levw + crap[p].xp) == 0)
			    || (*(mask + (crap[p].yp) * levw + crap[p].xp)
				== 255)
			    ) {
			    crap[p].active = 0;
            }
			break;
                }
                if (crap[p].timeleft > 0) {
                    crap[p].timeleft--;
                }
                if (crap[p].timeleft == 0) {
                    crap[p].active = 0;
                }
            }
        }

        /*
         * booms
         */
        for (p = 0; p < MAXBOOMS; p++) {
            if (boom[p].active)
            {
                if (((cnt % boom[p].speed) == 0) && (boom[p].frm > 0)) {
                    boom[p].frm++;
                }
                if (boom[p].frm >= boomanim[boom[p].type].frames) {
                    boom[p].frm = 0;
                }
                if (boom[p].transp == 0)
                {
                    for (a = 0; a < (MAXPLAYERS + MAXBOTS); a++) {
                        if ((plr[a].active) && (plr[a].energy > 0))
                        {
                            ox = boom[p].xp - (plr[a].xp + SHIPW / 2);
                            oy = boom[p].yp - (plr[a].yp + SHIPH / 2);
                            if ((ox < boomanim[boom[p].type].w)
                                && (oy < boomanim[boom[p].type].h))
                            {
                                b = boomanim[boom[p].type].w * 2 / 5;
                                if ((ox * ox + oy * oy) <= (b * b))
                                {
                                    plr[a].blink = 1;
                                    switch (boom[p].type)
                                    {
					case 0:
					    plr[a].energy -= FIXS / 8;
					    break;
					case 1:
					    plr[a].energy -= FIXS / 6;
					    break;
					case 2:
					    plr[a].energy -= FIXS / 4;
					    break;
					case 3:
					    plr[a].energy -= FIXS / 2;
					    break;
					case 4:
					    plr[a].energy -= FIXS / 6;
					    break;
					case 5:
					    plr[a].energy -= FIXS / 4;
					    break;
                                    }
                                    if (plr[a].energy < 0) {
                                        plr[a].energy = 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        /*
         * regenerate bases
         */
        if (cnt % REGENRATE == 0)
        {
            p = basepixcount = (basepixcount + 1) % basepixels;
            b = 1;
            for (a = 0; a < (MAXPLAYERS + MAXBOTS); a++) {
                if ((plr[a].active) && (plr[a].energy > 0))
                {
                    ox = (plr[a].xp + SHIPW / 2 - base[p].x);
                    oy = (plr[a].yp + SHIPH / 2 - base[p].y);
                    if ((ox * ox + oy * oy) < 256) {
                        b = 0;
                    }
                }
            }
            if (b)
            {
                *(level + base[p].y * levw + base[p].x) = base[p].c;
                *(mask + base[p].y * levw + base[p].x) = 0;
            }
        }

        /*
         * explosion when dead
         */
        for (p = 0; p < (MAXPLAYERS + MAXBOTS); p++) {
            if ((plr[p].active) && (oenergy[p] > 0))
            {
                if (plr[p].energy <= 0)
                {
                    dead++;
                    sound_eff(S_BSHIP, 64, 128, 16384, 1);
                    newboom(plr[p].xp + SHIPW / 2 + 19, plr[p].yp + SHIPH / 2, 1, 0, 3);
                    newboom(plr[p].xp + SHIPW / 2 - 19, plr[p].yp + SHIPH / 2, 1, 0, 3);
                    newboom(plr[p].xp + SHIPW / 2, plr[p].yp + SHIPH / 2 + 19, 1, 0, 3);
                    newboom(plr[p].xp + SHIPW / 2, plr[p].yp + SHIPH / 2 - 19, 1, 0, 3);
                    newboom(plr[p].xp + SHIPW / 2 + 14, plr[p].yp + SHIPH / 2 + 11, 1, 0, 3);
                    newboom(plr[p].xp + SHIPW / 2 - 14, plr[p].yp + SHIPH / 2 + 11, 1, 0, 3);
                    newboom(plr[p].xp + SHIPW / 2 + 14, plr[p].yp + SHIPH / 2 - 11, 1, 0, 3);
                    newboom(plr[p].xp + SHIPW / 2 - 14, plr[p].yp + SHIPH / 2 - 11, 1, 0, 3);
                    newboom(plr[p].xp + SHIPW / 2, plr[p].yp + SHIPH / 2, 3, 0, 4);
                    oenergy[p] = 0;
                }
            }
        }

        /*
         * f12 saves screen to SHOT.RIX
         */
#ifdef SCREENSHOTS
        if (key(K_F12))
        {
            gamepause = 1;
            savescreenrix("kopsshot.rix");
            while (key(K_F12))
            {
                update();
            }
            gamepause = 0;
        }
#endif

        /*
         * game pause
         */
        if ((key(K_F11)) || (waskey(K_F11)))
        {
            gamepause = 1;
            jsavestart();
            for (a = 0; a < 256; a++) {
                cdest[a * 3] = cdest[a * 3 + 1] = cdest[a * 3 + 2] =
                    (short) ((cstart[a * 3] + cstart[a * 3 + 1] + cstart[a * 3 + 2]) / 4);
            }
            jrealfade(0, 255, 7);
            while (key(K_F11))
            {
                update();
            }
            while (!key(K_F11))
            {
                update();
            }
            while (key(K_F11))
            {
                update();
            }
            for (a = 0; a < 768; a++) {
                cdest[a] = cstart[a];
            }
            jsavestart();
            jrealfade(0, 255, 7);
            clearkey(K_F11);
            gamepause = 0;
        }

        frm--;
    }
    drawscreen();
    /*
     * #ifdef VRC #ifdef RASTER jpal(0,0,0,0); #endif jvrc(); #ifdef
     * RASTER if (key(K_SPACE)) jpal(0,0,0,63); #endif #endif
     */
    updatescreen();

    /*
     * game start fadein
     */
    if (fade > 0)
    {
        jrealfade1(0, 255, 18);
        fade--;
    }

    /*
     * crappy falling water test ;-)
     */
    /*
     * for (a=(levh-1)*levw-1;a>=0;a--)
     { if
     * ((*(mask+a)>0)&&(*(mask+a)<8))
     { if (*(mask+a+levw)==0xff)
     {
     * *(level+a+levw)=*(mask+a+levw)=1; *(level+a)=0; *(mask+a)=0xff; }
     * else if (*(mask+a+levw+1)==0xff)
     {
     * *(level+a+levw+1)=*(mask+a+levw+1)=1; *(level+a)=0;
     * *(mask+a)=0xff; } else if (*(mask+a+levw-1)==0xff)
     {
     * *(level+a+levw-1)=*(mask+a+levw-1)=1; *(level+a)=0;
     * *(mask+a)=0xff; } } }
     */

}


jsavestart();
jcleardest();
jinitreal();
for (a = 8; a > 0; a--)
{
    jvrc();
    sound_pvol(a * 4);
    jrealfade1(0, 255, 8);
}
jvcfillbox(0, 0, 639, 479, 0);

update();

if (victory > 0)
{
    ++wins[victory - 1];
}

for (a = 0; a < (MAXPLAYERS + MAXPLAYERS); a++)
{
    /* if ((resurrect[a]==1)&&(plr[a].active==0)) initplayer(a); */
    if ((resurrect[a] == 1) && (plr[a].energy == 0)) {
        resetplayer2(a);
    } else {
        /*
         * player was a survivor..
         */
        for (b = 0; b < slots * 1 / 3; b++)
        {
            c = rand() % slots;
            plr[a].slots[c] = W_NOWEAPON;
            plr[a].bullets[c] = 0;
        }
        plr[a].hyperspac = 0;
        for (b = 0; b < slots; b++)
        {
            if (plr[a].slots[b] == W_HYPERSPAC)
            {
                plr[a].slots[b] = W_NOWEAPON;
                plr[a].bullets[b] = 0;
            }
        }
        plr[a].ammo2 = plr[a].slots[plr[a].activeslot];
    }
}
}
