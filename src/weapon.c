/* weapon.c
 * Copyright 2003
 *
 * $Id: weapon.c,v 1.1 2003/04/15 18:15:49 tonic Exp $
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

#include "global.h"
#include "gamefunc.h"
#include "sound.h"
#include "spr.h"
#include "kops-bot/reactive-agent/bot-util.h"

/* Weapon specials. */
static void ultraThrSpecial(int p);
static void retroSpecial(int p);
static void hyperSpacSpecial(int p);

/* Ammo move handlers. */
static void moveFalling(int p);
static void moveStraight(int p);
static void moveShaky(int p);
static void movePlasma(int p);
static void movePyrotech(int p);
static void moveToxicWast(int p);
static void moveToxicW2(int p);
static void moveLash(int p);
static void moveMine(int p);
static void moveHeatSeek(int p);
static void moveCrawler(int p);
static void moveDelBlast(int p);
static void moveSuperPyro(int p);

/* Ammo hit player handlers. */
static void hitPlain(int a, int p);
static void hitPlasma(int a, int p);
static void hitBlast(int a, int p);
static void hitBigBlast(int a, int p);
static void hitMine(int a, int p);
static void hitMegaBlast(int a, int p);
static void hitMissile(int a, int p);
static void hitMornStar(int a, int p);
static void hitSensorJam(int a, int p);
static void hitSignal(int a, int p);
static void hitFlipper(int a, int p);
static void hitBomb(int a, int p);
static void hitDelBlast(int a, int p);
static void hitGravFlow(int a, int p);
static void hitTractBeam(int a, int p);
static void hitSuperPyro(int a, int p);

/* Ammo hit handlers. */
static void ammoHitPlain(int p, int ox, int oy);
static void ammoHitMainGun(int p, int ox, int oy);
static void ammoHitToaster(int p, int ox, int oy);
static void ammoHitPlasma(int p, int ox, int oy);
static void ammoHitMornStar(int p, int ox, int oy);
static void ammoHitBlastGun(int p, int ox, int oy);
static void ammoHitBlaster(int p, int ox, int oy);
static void ammoHitMine(int p, int ox, int oy);
static void ammoHitMegaBlast(int p, int ox, int oy);
static void ammoHitMissile(int p, int ox, int oy);
static void ammoHitBomb(int p, int ox, int oy);
static void ammoHitPyro(int p, int ox, int oy);
static void ammoHitToxicWast(int p, int ox, int oy);
static void ammoHitLaser(int p, int ox, int oy);
static void ammoHitToxicW2(int p, int ox, int oy);

/* Pick ammo handlers. */
static void pickMain(int p, int ammo);
static void pickSpecial(int p, int ammo);
static void pickNextAmmo(int p, int ammo);

WEAPON_INFO weapons[W_TOTAL];

void initweapons()
{

    /* W_TUHNU     0 */
    weapons[W_TUHNU].firepause = 0;
    weapons[W_TUHNU].recoil = 0.0f;
    weapons[W_TUHNU].sampleno = 0;
    weapons[W_TUHNU].volume = 0;
    weapons[W_TUHNU].freq = 0;
    weapons[W_TUHNU].chgroup = 0;
    weapons[W_TUHNU].special = NULL;
    weapons[W_TUHNU].move = moveFalling;
    weapons[W_TUHNU].hit = hitPlain;
    weapons[W_TUHNU].ammohit = ammoHitMainGun;
    weapons[W_TUHNU].pick = pickMain;

    /* W_PEAGUN    1 */
    weapons[W_PEAGUN].firepause = 0;
    weapons[W_PEAGUN].recoil = 0.0f;
    weapons[W_PEAGUN].sampleno = 0;
    weapons[W_PEAGUN].volume = 0;
    weapons[W_PEAGUN].freq = 0;
    weapons[W_PEAGUN].chgroup = 0;
    weapons[W_PEAGUN].special = NULL;
    weapons[W_PEAGUN].move = moveFalling;
    weapons[W_PEAGUN].hit = hitPlain;
    weapons[W_PEAGUN].ammohit = ammoHitMainGun;
    weapons[W_PEAGUN].pick = pickMain;

    /* W_SLING     2 */
    weapons[W_SLING].firepause = 0;
    weapons[W_SLING].recoil = 0.0f;
    weapons[W_SLING].sampleno = 0;
    weapons[W_SLING].volume = 0;
    weapons[W_SLING].freq = 0;
    weapons[W_SLING].chgroup = 0;
    weapons[W_SLING].special = NULL;
    weapons[W_SLING].move = moveFalling;
    weapons[W_SLING].hit = hitPlain;
    weapons[W_SLING].ammohit = ammoHitMainGun;
    weapons[W_SLING].pick = pickMain;

    /* W_BIGPEAGUN 3 */
    weapons[W_BIGPEAGUN].firepause = 0;
    weapons[W_BIGPEAGUN].recoil = 0.0f;
    weapons[W_BIGPEAGUN].sampleno = 0;
    weapons[W_BIGPEAGUN].volume = 0;
    weapons[W_BIGPEAGUN].freq = 0;
    weapons[W_BIGPEAGUN].chgroup = 0;
    weapons[W_BIGPEAGUN].special = NULL;
    weapons[W_BIGPEAGUN].move = moveFalling;
    weapons[W_BIGPEAGUN].hit = hitPlain;
    weapons[W_BIGPEAGUN].ammohit = ammoHitMainGun;
    weapons[W_BIGPEAGUN].pick = pickMain;

    /* W_BIGSLING  4 */
    weapons[W_BIGSLING].firepause = 0;
    weapons[W_BIGSLING].recoil = 0.0f;
    weapons[W_BIGSLING].sampleno = 0;
    weapons[W_BIGSLING].volume = 0;
    weapons[W_BIGSLING].freq = 0;
    weapons[W_BIGSLING].chgroup = 0;
    weapons[W_BIGSLING].special = NULL;
    weapons[W_BIGSLING].move = moveFalling;
    weapons[W_BIGSLING].hit = hitPlain;
    weapons[W_BIGSLING].ammohit = ammoHitMainGun;
    weapons[W_BIGSLING].pick = pickMain;

    /* W_BLASTGUN  5 */
    weapons[W_BLASTGUN].firepause = 0;
    weapons[W_BLASTGUN].recoil = 0.0f;
    weapons[W_BLASTGUN].sampleno = 0;
    weapons[W_BLASTGUN].volume = 0;
    weapons[W_BLASTGUN].freq = 0;
    weapons[W_BLASTGUN].chgroup = 0;
    weapons[W_BLASTGUN].special = NULL;
    weapons[W_BLASTGUN].move = moveFalling;
    weapons[W_BLASTGUN].hit = hitPlain;
    weapons[W_BLASTGUN].ammohit = ammoHitBlastGun;
    weapons[W_BLASTGUN].pick = pickMain;

    /* W_MINIBLAST 6 */
    weapons[W_MINIBLAST].firepause = 0;
    weapons[W_MINIBLAST].recoil = 0.0f;
    weapons[W_MINIBLAST].sampleno = 0;
    weapons[W_MINIBLAST].volume = 0;
    weapons[W_MINIBLAST].freq = 0;
    weapons[W_MINIBLAST].chgroup = 0;
    weapons[W_MINIBLAST].special = NULL;
    weapons[W_MINIBLAST].move = moveFalling;
    weapons[W_MINIBLAST].hit = hitBlast;
    weapons[W_MINIBLAST].ammohit = ammoHitBlastGun;
    weapons[W_MINIBLAST].pick = pickMain;

    /* W_BLASTER   7 */
    weapons[W_BLASTER].firepause = 0;
    weapons[W_BLASTER].recoil = 0.0f;
    weapons[W_BLASTER].sampleno = 0;
    weapons[W_BLASTER].volume = 0;
    weapons[W_BLASTER].freq = 0;
    weapons[W_BLASTER].chgroup = 0;
    weapons[W_BLASTER].special = NULL;
    weapons[W_BLASTER].move = moveFalling;
    weapons[W_BLASTER].hit = hitBlast;
    weapons[W_BLASTER].ammohit = ammoHitBlaster;
    weapons[W_BLASTER].pick = pickMain;

    /* W_BIGBLASTR 8 */
    weapons[W_BIGBLASTR].firepause = 0;
    weapons[W_BIGBLASTR].recoil = 0.0f;
    weapons[W_BIGBLASTR].sampleno = 0;
    weapons[W_BIGBLASTR].volume = 0;
    weapons[W_BIGBLASTR].freq = 0;
    weapons[W_BIGBLASTR].chgroup = 0;
    weapons[W_BIGBLASTR].special = NULL;
    weapons[W_BIGBLASTR].move = moveFalling;
    weapons[W_BIGBLASTR].hit = hitBigBlast;
    weapons[W_BIGBLASTR].ammohit = ammoHitBlaster;
    weapons[W_BIGBLASTR].pick = pickMain;

    /*  special weapons etc start here */

    /* W_GASBLAST   9 */
    weapons[W_GASBLAST].firepause = 80;
    weapons[W_GASBLAST].recoil = 1.0f;
    weapons[W_GASBLAST].sampleno = S_GASBLAST;
    weapons[W_GASBLAST].volume = 64;
    weapons[W_GASBLAST].freq = 16384;
    weapons[W_GASBLAST].chgroup = 1;
    weapons[W_GASBLAST].special = NULL;
    weapons[W_GASBLAST].move = moveFalling;
    weapons[W_GASBLAST].hit = hitBigBlast;
    weapons[W_GASBLAST].ammohit = ammoHitMine;
    weapons[W_GASBLAST].pick = pickSpecial;

    /* W_MEGABLAST 10 */
    weapons[W_MEGABLAST].firepause = 90;
    weapons[W_MEGABLAST].recoil = 1.5f;
    weapons[W_MEGABLAST].sampleno = S_MEGABLAST;
    weapons[W_MEGABLAST].volume = 64;
    weapons[W_MEGABLAST].freq = 16384;
    weapons[W_MEGABLAST].chgroup = 1;
    weapons[W_MEGABLAST].special = NULL;
    weapons[W_MEGABLAST].move = moveFalling;
    weapons[W_MEGABLAST].hit = hitMegaBlast;
    weapons[W_MEGABLAST].ammohit = ammoHitMegaBlast;
    weapons[W_MEGABLAST].pick = pickSpecial;

    /* W_MINE      11 */
    weapons[W_MINE].firepause = 140;
    weapons[W_MINE].recoil = 0.0f;
    weapons[W_MINE].sampleno = S_MINE;
    weapons[W_MINE].volume = 64;
    weapons[W_MINE].freq = 16384;
    weapons[W_MINE].chgroup = 1;
    weapons[W_MINE].special = NULL;
    weapons[W_MINE].move = moveMine;
    weapons[W_MINE].hit = hitMine;
    weapons[W_MINE].ammohit = ammoHitMine;
    weapons[W_MINE].pick = pickSpecial;

    /* W_HEATSEEK  12 */
    weapons[W_HEATSEEK].firepause = 125;
    weapons[W_HEATSEEK].recoil = 0.0f;
    weapons[W_HEATSEEK].sampleno = S_HEATSEEK;
    weapons[W_HEATSEEK].volume = 64;
    weapons[W_HEATSEEK].freq = 16384;
    weapons[W_HEATSEEK].chgroup = 1;
    weapons[W_HEATSEEK].special = NULL;
    weapons[W_HEATSEEK].move = moveHeatSeek;
    weapons[W_HEATSEEK].hit = hitMissile;
    weapons[W_HEATSEEK].ammohit = ammoHitMissile;
    weapons[W_HEATSEEK].pick = pickSpecial;

    /* W_MORNSTAR  13 */
    weapons[W_MORNSTAR].firepause = 140;
    weapons[W_MORNSTAR].recoil = 1.0f;
    weapons[W_MORNSTAR].sampleno = S_MORNSTAR;
    weapons[W_MORNSTAR].volume = 64;
    weapons[W_MORNSTAR].freq = 16384;
    weapons[W_MORNSTAR].chgroup = 1;
    weapons[W_MORNSTAR].special = NULL;
    weapons[W_MORNSTAR].move = moveFalling;
    weapons[W_MORNSTAR].hit = hitMornStar;
    weapons[W_MORNSTAR].ammohit = ammoHitMornStar;
    weapons[W_MORNSTAR].pick = pickSpecial;

    /* W_SENSORJAM 14 */
    weapons[W_SENSORJAM].firepause = 120;
    weapons[W_SENSORJAM].recoil = 0.0f;
    weapons[W_SENSORJAM].sampleno = S_SENSORJAM;
    weapons[W_SENSORJAM].volume = 64;
    weapons[W_SENSORJAM].freq = 16384;
    weapons[W_SENSORJAM].chgroup = 1;
    weapons[W_SENSORJAM].special = NULL;
    weapons[W_SENSORJAM].move = moveFalling;
    weapons[W_SENSORJAM].hit = hitSensorJam;
    weapons[W_SENSORJAM].ammohit = ammoHitPlain;
    weapons[W_SENSORJAM].pick = pickSpecial;

    /* W_NOWEAPON  15 */
    weapons[W_NOWEAPON].firepause = 0;
    weapons[W_NOWEAPON].recoil = 0.0f;
    weapons[W_NOWEAPON].sampleno = 0;
    weapons[W_NOWEAPON].volume = 0;
    weapons[W_NOWEAPON].freq = 0;
    weapons[W_NOWEAPON].chgroup = 0;
    weapons[W_NOWEAPON].special = NULL;
    weapons[W_NOWEAPON].move = NULL;
    weapons[W_NOWEAPON].hit = NULL;
    weapons[W_NOWEAPON].ammohit = NULL;
    weapons[W_NOWEAPON].pick = pickSpecial;

    /* W_PYROTECH  16 */
    weapons[W_PYROTECH].firepause = 600;
    weapons[W_PYROTECH].recoil = 0.0f;
    weapons[W_PYROTECH].sampleno = S_PYROTECH;
    weapons[W_PYROTECH].volume = 64;
    weapons[W_PYROTECH].freq = 16384;
    weapons[W_PYROTECH].chgroup = 1;
    weapons[W_PYROTECH].special = NULL;
    weapons[W_PYROTECH].move = movePyrotech;
    weapons[W_PYROTECH].hit = hitBomb;
    weapons[W_PYROTECH].ammohit = ammoHitPyro;
    weapons[W_PYROTECH].pick = pickSpecial;

    /* W_TOXICWAST 17 */
    weapons[W_TOXICWAST].firepause = 600;
    weapons[W_TOXICWAST].recoil = 0.0f;
    weapons[W_TOXICWAST].sampleno = S_TOXICWAST;
    weapons[W_TOXICWAST].volume = 64;
    weapons[W_TOXICWAST].freq = 16384;
    weapons[W_TOXICWAST].chgroup = 1;
    weapons[W_TOXICWAST].special = NULL;
    weapons[W_TOXICWAST].move = moveToxicWast;
    weapons[W_TOXICWAST].hit = hitMissile;
    weapons[W_TOXICWAST].ammohit = ammoHitToxicWast;
    weapons[W_TOXICWAST].pick = pickSpecial;

    /* W_TOASTER   18 */
    weapons[W_TOASTER].firepause = 2;
    weapons[W_TOASTER].recoil = 0.0f;
    weapons[W_TOASTER].sampleno = S_TOASTER;
    weapons[W_TOASTER].volume = 64;
    weapons[W_TOASTER].freq = 16384;
    weapons[W_TOASTER].chgroup = 0;
    weapons[W_TOASTER].special = NULL;
    weapons[W_TOASTER].move = moveShaky;
    weapons[W_TOASTER].hit = hitPlain;
    weapons[W_TOASTER].ammohit = ammoHitToaster;
    weapons[W_TOASTER].pick = pickSpecial;

    /* W_REARSLING 19 */
    weapons[W_REARSLING].firepause = 10;
    weapons[W_REARSLING].recoil = 0.0f;
    weapons[W_REARSLING].sampleno = S_REARSLING;
    weapons[W_REARSLING].volume = 64;
    weapons[W_REARSLING].freq = 16384;
    weapons[W_REARSLING].chgroup = 0;
    weapons[W_REARSLING].special = NULL;
    weapons[W_REARSLING].move = moveFalling;
    weapons[W_REARSLING].hit = hitPlain;
    weapons[W_REARSLING].ammohit = ammoHitMainGun;
    weapons[W_REARSLING].pick = pickSpecial;

    /* W_RINGFIRE  20 */
    weapons[W_RINGFIRE].firepause = 120;
    weapons[W_RINGFIRE].recoil = 0.0f;
    weapons[W_RINGFIRE].sampleno = S_RINGFIRE;
    weapons[W_RINGFIRE].volume = 64;
    weapons[W_RINGFIRE].freq = 16384;
    weapons[W_RINGFIRE].chgroup = 1;
    weapons[W_RINGFIRE].special = NULL;
    weapons[W_RINGFIRE].move = NULL;
    weapons[W_RINGFIRE].hit = NULL;
    weapons[W_RINGFIRE].ammohit = NULL;
    weapons[W_RINGFIRE].pick = pickSpecial;

    /* W_CRAWLER   21 */
    weapons[W_CRAWLER].firepause = 90;
    weapons[W_CRAWLER].recoil = 0.5f;
    weapons[W_CRAWLER].sampleno = S_CRAWLER;
    weapons[W_CRAWLER].volume = 64;
    weapons[W_CRAWLER].freq = 16384;
    weapons[W_CRAWLER].chgroup = 1;
    weapons[W_CRAWLER].special = NULL;
    weapons[W_CRAWLER].move = moveCrawler;
    weapons[W_CRAWLER].hit = hitMissile;
    weapons[W_CRAWLER].ammohit = ammoHitMissile;
    weapons[W_CRAWLER].pick = pickSpecial;

    /* W_GRAVFLOW  22 */
    weapons[W_GRAVFLOW].firepause = 4;
    weapons[W_GRAVFLOW].recoil = 0.0f;
    weapons[W_GRAVFLOW].sampleno = S_GRAVFLOW;
    weapons[W_GRAVFLOW].volume = 64;
    weapons[W_GRAVFLOW].freq = 16384;
    weapons[W_GRAVFLOW].chgroup = 0;
    weapons[W_GRAVFLOW].special = NULL;
    weapons[W_GRAVFLOW].move = moveShaky;
    weapons[W_GRAVFLOW].hit = hitGravFlow;
    weapons[W_GRAVFLOW].ammohit = ammoHitPlain;
    weapons[W_GRAVFLOW].pick = pickSpecial;

    /* W_BOMB      23 */
    weapons[W_BOMB].firepause = 80;
    weapons[W_BOMB].recoil = 0.0f;
    weapons[W_BOMB].sampleno = S_BOMB;
    weapons[W_BOMB].volume = 64;
    weapons[W_BOMB].freq = 16384;
    weapons[W_BOMB].chgroup = 1;
    weapons[W_BOMB].special = NULL;
    weapons[W_BOMB].move = moveFalling;
    weapons[W_BOMB].hit = hitBomb;
    weapons[W_BOMB].ammohit = ammoHitBomb;
    weapons[W_BOMB].pick = pickSpecial;

    /* W_LASH      24 */
    weapons[W_LASH].firepause = 100;
    weapons[W_LASH].recoil = 1.0f;
    weapons[W_LASH].sampleno = S_LASH;
    weapons[W_LASH].volume = 64;
    weapons[W_LASH].freq = 16384;
    weapons[W_LASH].chgroup = 1;
    weapons[W_LASH].special = NULL;
    weapons[W_LASH].move = moveLash;
    weapons[W_LASH].hit = hitPlain;
    weapons[W_LASH].ammohit = ammoHitBlaster;
    weapons[W_LASH].pick = pickSpecial;

    /* W_ULTRATHR  25 */
    weapons[W_ULTRATHR].firepause = 100;
    weapons[W_ULTRATHR].recoil = (float)MAXSPEED / FIXS;
    weapons[W_ULTRATHR].sampleno = S_ULTRATHR;
    weapons[W_ULTRATHR].volume = 64;
    weapons[W_ULTRATHR].freq = 16384;
    weapons[W_ULTRATHR].chgroup = 1;
    weapons[W_ULTRATHR].special = ultraThrSpecial;
    weapons[W_ULTRATHR].move = NULL;
    weapons[W_ULTRATHR].hit = NULL;
    weapons[W_ULTRATHR].ammohit = NULL;
    weapons[W_ULTRATHR].pick = pickSpecial;

    /* W_RETRO     26 */
    weapons[W_RETRO].firepause = 5;
    weapons[W_RETRO].recoil = (float)ACCELDIV / 3;
    weapons[W_RETRO].sampleno = S_RETRO;
    weapons[W_RETRO].volume = 64;
    weapons[W_RETRO].freq = 16384;
    weapons[W_RETRO].chgroup = 0;
    weapons[W_RETRO].special = retroSpecial;
    weapons[W_RETRO].move = NULL;
    weapons[W_RETRO].hit = NULL;
    weapons[W_RETRO].ammohit = NULL;
    weapons[W_RETRO].pick = pickSpecial;

    /* W_HYPERSPAC 27 */
    weapons[W_HYPERSPAC].firepause = 70;
    weapons[W_HYPERSPAC].recoil = 0.0f;
    weapons[W_HYPERSPAC].sampleno = S_HYPERSPAC;
    weapons[W_HYPERSPAC].volume = 64;
    weapons[W_HYPERSPAC].freq = 16384;
    weapons[W_HYPERSPAC].chgroup = 1;
    weapons[W_HYPERSPAC].special = NULL;
    weapons[W_HYPERSPAC].move = hyperSpacSpecial;
    weapons[W_HYPERSPAC].hit = NULL;
    weapons[W_HYPERSPAC].ammohit = NULL;
    weapons[W_HYPERSPAC].pick = pickSpecial;

    /* W_PLASMA    28 */
    weapons[W_PLASMA].firepause = 280;
    weapons[W_PLASMA].recoil = 1.0f;
    weapons[W_PLASMA].sampleno = S_PLASMA;
    weapons[W_PLASMA].volume = 64;
    weapons[W_PLASMA].freq = 16384;
    weapons[W_PLASMA].chgroup = 1;
    weapons[W_PLASMA].special = NULL;
    weapons[W_PLASMA].move = movePlasma;
    weapons[W_PLASMA].hit = hitPlasma;
    weapons[W_PLASMA].ammohit = ammoHitPlasma;
    weapons[W_PLASMA].pick = pickSpecial;

    /* W_PULSLASER 29 */
    weapons[W_PULSLASER].firepause = 13;
    weapons[W_PULSLASER].recoil = 0.0f;
    weapons[W_PULSLASER].sampleno = S_PULSLASER;
    weapons[W_PULSLASER].volume = 64;
    weapons[W_PULSLASER].freq = 16384;
    weapons[W_PULSLASER].chgroup = 0;
    weapons[W_PULSLASER].special = NULL;
    weapons[W_PULSLASER].move = moveStraight;
    weapons[W_PULSLASER].hit = hitPlain;
    weapons[W_PULSLASER].ammohit = ammoHitLaser;
    weapons[W_PULSLASER].pick = pickSpecial;

    /* W_DELBLAST  30 */
    weapons[W_DELBLAST].firepause = 300;
    weapons[W_DELBLAST].recoil = 0.0f;
    weapons[W_DELBLAST].sampleno = S_DELBLAST;
    weapons[W_DELBLAST].volume = 64;
    weapons[W_DELBLAST].freq = 16384;
    weapons[W_DELBLAST].chgroup = 1;
    weapons[W_DELBLAST].special = NULL;
    weapons[W_DELBLAST].move = moveDelBlast;
    weapons[W_DELBLAST].hit = hitDelBlast;
    weapons[W_DELBLAST].ammohit = NULL;
    weapons[W_DELBLAST].pick = pickSpecial;

    /* W_FLIPPER   31 */
    weapons[W_FLIPPER].firepause = 150;
    weapons[W_FLIPPER].recoil = 0.0f;
    weapons[W_FLIPPER].sampleno = S_FLIPPER;
    weapons[W_FLIPPER].volume = 64;
    weapons[W_FLIPPER].freq = 16384;
    weapons[W_FLIPPER].chgroup = 1;
    weapons[W_FLIPPER].special = NULL;
    weapons[W_FLIPPER].move = moveFalling;
    weapons[W_FLIPPER].hit = hitFlipper;
    weapons[W_FLIPPER].ammohit = ammoHitPlain;
    weapons[W_FLIPPER].pick = pickSpecial;

    /* W_SIGNAL    32 */
    weapons[W_SIGNAL].firepause = 180;
    weapons[W_SIGNAL].recoil = 0.0f;
    weapons[W_SIGNAL].sampleno = S_SIGNAL;
    weapons[W_SIGNAL].volume = 64;
    weapons[W_SIGNAL].freq = 16384;
    weapons[W_SIGNAL].chgroup = 1;
    weapons[W_SIGNAL].special = NULL;
    weapons[W_SIGNAL].move = moveFalling;
    weapons[W_SIGNAL].hit = hitSignal;
    weapons[W_SIGNAL].ammohit = ammoHitPlain;
    weapons[W_SIGNAL].pick = pickSpecial;

    /* W_NEXTAMMO  33 */
    weapons[W_NEXTAMMO].firepause = 0;
    weapons[W_NEXTAMMO].recoil = 0.0f;
    weapons[W_NEXTAMMO].sampleno = 0;
    weapons[W_NEXTAMMO].volume = 0;
    weapons[W_NEXTAMMO].freq = 0;
    weapons[W_NEXTAMMO].chgroup = 0;
    weapons[W_NEXTAMMO].special = NULL;
    weapons[W_NEXTAMMO].move = NULL;
    weapons[W_NEXTAMMO].hit = NULL;
    weapons[W_NEXTAMMO].ammohit = NULL;
    weapons[W_NEXTAMMO].pick = pickNextAmmo;

    /* W_TUHNRUISK 34 */
    weapons[W_TUHNRUISK].firepause = 3;
    weapons[W_TUHNRUISK].recoil = 0.0f;
    weapons[W_TUHNRUISK].sampleno = S_TUHNU;
    weapons[W_TUHNRUISK].volume = 64;
    weapons[W_TUHNRUISK].freq = 16384;
    weapons[W_TUHNRUISK].chgroup = 0;
    weapons[W_TUHNRUISK].special = NULL;
    weapons[W_TUHNRUISK].move = NULL;
    weapons[W_TUHNRUISK].hit = NULL;
    weapons[W_TUHNRUISK].ammohit = NULL;
    weapons[W_TUHNRUISK].pick = pickSpecial;

    /* W_HYPERBLST 35 */
    weapons[W_HYPERBLST].firepause = 3;
    weapons[W_HYPERBLST].recoil = 1.0f / 7.0f;
    weapons[W_HYPERBLST].sampleno = S_GASBLAST;
    weapons[W_HYPERBLST].volume = 64;
    weapons[W_HYPERBLST].freq = 16384;
    weapons[W_HYPERBLST].chgroup = 0;
    weapons[W_HYPERBLST].special = NULL;
    weapons[W_HYPERBLST].move = NULL;
    weapons[W_HYPERBLST].hit = NULL;
    weapons[W_HYPERBLST].ammohit = NULL;
    weapons[W_HYPERBLST].pick = pickSpecial;

    /* W_TRACTBEAM 36 */
    weapons[W_TRACTBEAM].firepause = 5;
    weapons[W_TRACTBEAM].recoil = 0.0f;
    weapons[W_TRACTBEAM].sampleno = S_GRAVFLOW;
    weapons[W_TRACTBEAM].volume = 64;
    weapons[W_TRACTBEAM].freq = 16384;
    weapons[W_TRACTBEAM].chgroup = 0;
    weapons[W_TRACTBEAM].special = NULL;
    weapons[W_TRACTBEAM].move = moveShaky;
    weapons[W_TRACTBEAM].hit = hitTractBeam;
    weapons[W_TRACTBEAM].ammohit = ammoHitPlain;
    weapons[W_TRACTBEAM].pick = pickSpecial;

    /* W_TWINLASER 37 */
    weapons[W_TWINLASER].firepause = 18;
    weapons[W_TWINLASER].recoil = 0.0f;
    weapons[W_TWINLASER].sampleno = S_PULSLASER;
    weapons[W_TWINLASER].volume = 64;
    weapons[W_TWINLASER].freq = 16384;
    weapons[W_TWINLASER].chgroup = 0;
    weapons[W_TWINLASER].special = NULL;
    weapons[W_TWINLASER].move = NULL;
    weapons[W_TWINLASER].hit = NULL;
    weapons[W_TWINLASER].ammohit = NULL;
    weapons[W_TWINLASER].pick = pickSpecial;

    /* W_SUPERPYRO 38 */
    weapons[W_SUPERPYRO].firepause = 700;
    weapons[W_SUPERPYRO].recoil = 0.0f;
    weapons[W_SUPERPYRO].sampleno = S_PYROTECH;
    weapons[W_SUPERPYRO].volume = 64;
    weapons[W_SUPERPYRO].freq = 16384;
    weapons[W_SUPERPYRO].chgroup = 1;
    weapons[W_SUPERPYRO].special = NULL;
    weapons[W_SUPERPYRO].move = moveSuperPyro;
    weapons[W_SUPERPYRO].hit = hitSuperPyro;
    weapons[W_SUPERPYRO].ammohit = ammoHitPyro;
    weapons[W_SUPERPYRO].pick = pickSpecial;

    /* W_SPILLER   39 */
    weapons[W_SPILLER].firepause = 7;
    weapons[W_SPILLER].recoil = 0.0f;
    weapons[W_SPILLER].sampleno = S_BLASTGUN;
    weapons[W_SPILLER].volume = 64;
    weapons[W_SPILLER].freq = 16384;
    weapons[W_SPILLER].chgroup = 0;
    weapons[W_SPILLER].special = NULL;
    weapons[W_SPILLER].move = NULL;
    weapons[W_SPILLER].hit = NULL;
    weapons[W_SPILLER].ammohit = NULL;
    weapons[W_SPILLER].pick = pickSpecial;

    /* W_TOXICW2   40 */
    weapons[W_TOXICW2].firepause = 0;
    weapons[W_TOXICW2].recoil = 0.0f;
    weapons[W_TOXICW2].sampleno = 0;
    weapons[W_TOXICW2].volume = 0;
    weapons[W_TOXICW2].freq = 0;
    weapons[W_TOXICW2].chgroup = 0;
    weapons[W_TOXICW2].special = NULL;
    weapons[W_TOXICW2].move = moveToxicW2;
    weapons[W_TOXICW2].hit = hitBlast;
    weapons[W_TOXICW2].ammohit = ammoHitToxicW2;
    weapons[W_TOXICW2].pick = pickSpecial;

    /* W_DOT       41 */
    weapons[W_DOT].firepause = 0;
    weapons[W_DOT].recoil = 0.0f;
    weapons[W_DOT].sampleno = 0;
    weapons[W_DOT].volume = 0;
    weapons[W_DOT].freq = 0;
    weapons[W_DOT].chgroup = 0;
    weapons[W_DOT].special = NULL;
    weapons[W_DOT].move = moveFalling;
    weapons[W_DOT].hit = hitPlain;
    weapons[W_DOT].ammohit = ammoHitLaser;
    weapons[W_DOT].pick = pickSpecial;

    /* End of Records. */
};

static void ultraThrSpecial(int p)
{

    int a;

    for (a = 0; a < 15; a++) {
        newplrcrap(p, 0);
        newplrcrap(p, 4);
    }
}

static void retroSpecial(int p)
{
    newplrcrap(p, 0);
}

static void hyperSpacSpecial(int p)
{
    plr[p].hyperspac = (Uint8)(1 - plr[p].hyperspac);
}

/**
 * Ammo movement routines.
 */
static void moveFalling(int p)
{
    ammo[p].yi += ammogravity;
    ammo[p].x += ammo[p].xi;
    ammo[p].y += ammo[p].yi;
}

static void moveStraight(int p)
{
    ammo[p].x += ammo[p].xi;
    ammo[p].y += ammo[p].yi;
}

static void moveShaky(int p)
{
    ammo[p].x += ammo[p].xi;
    ammo[p].y += ammo[p].yi;
    ammo[p].xi += rand() % (FIXS / 2) - FIXS / 4;
    ammo[p].yi += rand() % (FIXS / 2) - FIXS / 4;
}

static void movePlasma(int p)
{
    ammo[p].x += ammo[p].xi;
    ammo[p].y += ammo[p].yi;
    ammo[p].hitdist = (((sint[(cnt * 12) & (SCT - 1)] * 10) >> FIXP) + 40) / 2;

    if (ammo[p].timeleft == 4500) {
        ammo[p].damage = FIXS * 2;
    }
}

static void movePyrotech(int p)
{

    if (ammo[p].timeleft > 375) {
        ammo[p].yi += ammogravity;
        ammo[p].x += ammo[p].xi;
        ammo[p].y += ammo[p].yi;
    } else {
        if ((cnt & 3) == 0) {
            if ((*(mask + (ammo[p].yp - 4) * levw + ammo[p].xp) > 0) && (*(mask + (ammo[p].yp - 4) * levw + ammo[p].xp) < 8)) {
                if (rand() % 5 == 0) {
                    newammocrap(p, 3);
                }
            } else {
                if ((cnt & 31) == 0) {
                    sound_eff(S_MINIBLAST, 32, 128, 16384, 0);
                }
                newammoa(p);
            }
        }
    }
}

static void moveToxicWast(int p)
{
    if (ammo[p].timeleft > 600) {
        ammo[p].yi += ammogravity;
        ammo[p].x += ammo[p].xi;
        ammo[p].y += ammo[p].yi;
    } else {
        if ((cnt & 7) == 0) {
            sound_eff(S_HEATSTIK, 32, 128, 8192, 0);
            newammoa(p);
        }
        if (((cnt & 7) == 3) && (rand() % 5 == 0)) {
            newammocrap(p, 3);
        }
    }
}

static void moveToxicW2(int p)
{
    ammo[p].x += ammo[p].xi;
    ammo[p].y += ammo[p].yi;
    ammo[p].xp = ammo[p].x >> FIXP;
    ammo[p].yp = ammo[p].y >> FIXP;
    ammo[p].xi += rand() % (FIXS / 12) - FIXS / 24;
    ammo[p].yi += rand() % (FIXS / 12) - FIXS / 24;
    if (*(mask + (ammo[p].yp) * levw + ammo[p].xp) == 255) {
        ammo[p].active = 0;
    }
}

static void moveLash(int p)
{
    ammo[p].yi += ammogravity;
    ammo[p].x += ammo[p].xi;
    ammo[p].y += ammo[p].yi;
    if ((cnt & 3) == 0) {
        if ((cnt & 7) == 0) {
            sound_eff(S_LASHTIK, 64, 128, 16384, 0);
        }
        newammoa(p);
    }
}

static void moveMine(int p)
{
    ammo[p].xi = (ammo[p].xi + 8) % SCT;
    ammo[p].y = ammo[p].yi + (sint[ammo[p].xi] << 3);
    if (ammo[p].timeleft == 22400) {
        ammo[p].hitdist = 12;
    }
}

static void moveHeatSeek(int p)
{

    int a, b, c, d;
    int ox, oy;

    c = 0;

    if ((cnt & 15) == 0) {
        sound_eff(S_HEATSTIK, 64, 128, 16384, 0);
    }

    ammo[p].x += ammo[p].xi;
    ammo[p].y += ammo[p].yi;

    if (ammo[p].timeleft < 9000) {
        b = 9999999;
        for (a = 0; a < MAXPLAYERS; a++) {
            if ((plr[a].active) && (plr[a].energy > 0)) {
                ox = ammo[p].xp - (plr[a].xp + SHIPW / 2);
                oy = ammo[p].yp - (plr[a].yp + SHIPH / 2);
                if ((d = (ox * ox + oy * oy)) < b) {
                    b = d;
                    c = a;
                }
            }
        }
        ox = ammo[p].xp - (plr[c].xp + SHIPW / 2);
        oy = ammo[p].yp - (plr[c].yp + SHIPH / 2);
        a = isqrt(ox * ox + oy * oy);
        ammo[p].xi -= (ox << (FIXP - 2)) / a;
        ammo[p].yi -= (oy << (FIXP - 2)) / a;
        ammo[p].xi = (ammo[p].xi * 234) >> 8;
        ammo[p].yi = (ammo[p].yi * 234) >> 8;
    }
}

static void moveCrawler(int p)
{

    int a, b, c, d;
    int ox, oy;

    c = 0;
    ammo[p].x += ammo[p].xi;
    ammo[p].y += ammo[p].yi;
    if (ammo[p].timeleft < 22500) {
        b = 9999999;
        for (a = 0; a < MAXPLAYERS; a++) {
            if ((plr[a].active) && (plr[a].energy > 0)) {
                ox = ammo[p].xp - (plr[a].xp + SHIPW / 2);
                oy = ammo[p].yp - (plr[a].yp + SHIPH / 2);
                if ((d = (ox * ox + oy * oy)) < b) {
                    b = d;
                    c = a;
                }
            }
        }

        ox = ammo[p].xp - (plr[c].xp + SHIPW / 2);
        oy = ammo[p].yp - (plr[c].yp + SHIPH / 2);
        a = isqrt(ox * ox + oy * oy);

        ammo[p].xi -= (ox << (FIXP - 2)) / a;
        ammo[p].yi -= (oy << (FIXP - 2)) / a;
        ammo[p].xi = (ammo[p].xi * 3) >> 2;
        ammo[p].yi = (ammo[p].yi * 3) >> 2;
    }
}

static void moveDelBlast(int p)
{
    if (ammo[p].timeleft == 1) {
        sound_eff(S_BDELBL, 64, 128, 16384, 1);
        newboom(ammo[p].xp + 10, ammo[p].yp - 30, 0, 0, 2);
        newboom(ammo[p].xp - 10, ammo[p].yp - 30, 0, 0, 2);
        newboom(ammo[p].xp + 30, ammo[p].yp - 10, 0, 0, 2);
        newboom(ammo[p].xp + 30, ammo[p].yp + 10, 0, 0, 2);
        newboom(ammo[p].xp + 10, ammo[p].yp + 30, 0, 0, 2);
        newboom(ammo[p].xp - 10, ammo[p].yp + 30, 0, 0, 2);
        newboom(ammo[p].xp - 30, ammo[p].yp - 10, 0, 0, 2);
        newboom(ammo[p].xp - 30, ammo[p].yp + 10, 0, 0, 2);
        newboom(ammo[p].xp + 22, ammo[p].yp, 2, 0, 3);
        newboom(ammo[p].xp, ammo[p].yp + 22, 2, 0, 3);
        newboom(ammo[p].xp - 22, ammo[p].yp, 2, 0, 3);
        newboom(ammo[p].xp, ammo[p].yp - 22, 2, 0, 3);
        newboom(ammo[p].xp + 17, ammo[p].yp + 17, 1, 0, 3);
        newboom(ammo[p].xp + 17, ammo[p].yp - 17, 1, 0, 3);
        newboom(ammo[p].xp - 17, ammo[p].yp + 17, 1, 0, 3);
        newboom(ammo[p].xp - 17, ammo[p].yp - 17, 1, 0, 3);
        newboom(ammo[p].xp, ammo[p].yp, 3, 0, 4);
    }
}

static void moveSuperPyro(int p)
{
    if (ammo[p].timeleft > 375) {
        ammo[p].yi += ammogravity;
        ammo[p].x += ammo[p].xi;
        ammo[p].y += ammo[p].yi;
    } else {
        if ((cnt & 3) == 0) {
            if ((*(mask + (ammo[p].yp - 4) * levw + ammo[p].xp) > 0) && (*(mask + (ammo[p].yp - 4) * levw + ammo[p].xp) < 8)) {
                if (rand() % 5 == 0) {
                    newammocrap(p, 3);
                }
            } else {
                if ((cnt & 31) == 0) {
                    sound_eff(S_GASBLAST, 32, 128, 16384, 0);
                }
                newammoa(p);
            }
        }
    }
}

static void hitPlain(int a, int p)
{
    ammo[p].active = 0;
}

static void hitPlasma(int a, int p)
{
    if (ammo[p].timeleft > 4500) {
        plr[a].blink = 0;
    }
}

static void hitBlast(int a, int p)
{
    if (ammo[p].type == W_BLASTER) {
        sound_eff(S_BBLAST, 32, 128, 16384, 0);
    }
    if ((ammo[p].type == W_MINIBLAST) || (ammo[p].type == W_TOXICW2)) {
        sound_eff(S_BMINIB, 26, 128, 16384, 0);
    }
    newboom(ammo[p].xp, ammo[p].yp, 0, 1, 2);
    ammo[p].active = 0;
}

static void hitBigBlast(int a, int p)
{
    if (ammo[p].type == W_BIGBLASTR) {
        sound_eff(S_BBIGBL, 32, 128, 16384, 1);
    }
    if (ammo[p].type == W_GASBLAST) {
        sound_eff(S_BGASBL, 32, 128, 16384, 1);
    }
    plr[a].xi = (plr[a].xi * 7 + ammo[p].xi) / 8;
    plr[a].yi = (plr[a].yi * 7 + ammo[p].yi) / 8;
    newboom(ammo[p].xp, ammo[p].yp, 1, 1, 2);
    ammo[p].active = 0;
}

static void hitMine(int a, int p)
{
    sound_eff(S_BGASBL, 32, 128, 16384, 1);
    newboom(ammo[p].xp, ammo[p].yp, 1, 1, 2);
    ammo[p].active = 0;
}

static void hitMegaBlast(int a, int p)
{
    sound_eff(S_BMEGAB, 32, 128, 16384, 1);
    plr[a].xi = (plr[a].xi * 3 + ammo[p].xi) / 4;
    plr[a].yi = (plr[a].yi * 3 + ammo[p].yi) / 4;
    newboom(ammo[p].xp, ammo[p].yp, 2, 1, 2);
    ammo[p].active = 0;
}

static void hitMissile(int a, int p)
{
    sound_eff(S_BMINIB, 32, 128, 16384, 1);
    plr[a].xi = (plr[a].xi * 7 + ammo[p].xi) / 8;
    plr[a].yi = (plr[a].yi * 7 + ammo[p].yi) / 8;
    newboom(ammo[p].xp, ammo[p].yp, 4, 1, 2);
    ammo[p].active = 0;
}

static void hitMornStar(int a, int p)
{
    sound_eff(S_BBLAST, 32, 128, 16384, 1);
    plr[a].xi = (plr[a].xi * 7 + ammo[p].xi) / 8;
    plr[a].yi = (plr[a].yi * 7 + ammo[p].yi) / 8;
    // newammoa(p);
    newboom(ammo[p].xp, ammo[p].yp, 5, 1, 2);
    ammo[p].active = 0;
}

static void hitSensorJam(int a, int p)
{
    plr[a].sensorjam += 750;
    ammo[p].active = 0;
}

static void hitSignal(int a, int p)
{
    plr[a].signal += 1500;
    plr[a].flip = 0;
    ammo[p].active = 0;
}

static void hitFlipper(int a, int p)
{
    plr[a].signal = 0;
    plr[a].flip += 450;
    ammo[p].active = 0;
}

static void hitBomb(int a, int p)
{
    if (ammo[p].type == W_BOMB) {
        sound_eff(S_BBOMB, 32, 128, 16384, 1);
    }
    if (ammo[p].type == W_PYROTECH) {
        sound_eff(S_BMINIB, 32, 128, 16384, 1);
    }
    plr[a].xi = (plr[a].xi * 3 + ammo[p].xi) / 4;
    plr[a].yi = (plr[a].yi * 3 + ammo[p].yi) / 4;
    newboom(ammo[p].xp, ammo[p].yp, 4, 1, 3);
    ammo[p].active = 0;
}

static void hitDelBlast(int a, int p)
{
    sound_eff(S_BDELBL, 32, 128, 16384, 1);
    newboom(ammo[p].xp, ammo[p].yp, 3, 1, 3);
    ammo[p].active = 0;
}

static void hitGravFlow(int a, int p)
{
    plr[a].xi = (plr[a].xi + ammo[p].xi) / 2;
    plr[a].yi = (plr[a].yi + ammo[p].yi) / 2;
    ammo[p].active = 0;
}

static void hitTractBeam(int a, int p)
{
    plr[a].xi = (plr[a].xi * 7 - ammo[p].xi) / 8;
    plr[a].yi = (plr[a].yi * 7 - ammo[p].yi) / 8;
    ammo[p].active = 0;
}

static void hitSuperPyro(int a, int p)
{
    sound_eff(S_BMINIB, 42, 128, 16384, 1);
    plr[a].xi = (plr[a].xi * 3 + ammo[p].xi) / 4;
    plr[a].yi = (plr[a].yi * 3 + ammo[p].yi) / 4;
    newboom(ammo[p].xp, ammo[p].yp, 2, 1, 3);
    ammo[p].active = 0;
}

static void ammoHitPlain(int p, int ox, int oy)
{
    ammo[p].active = 0;
}

static void ammoHitMainGun(int p, int ox, int oy)
{

    newammocrap(p, 2);

    exline(ammo[p].xp, ammo[p].yp, ammo[p].oxp, ammo[p].oyp, 0, 0, levw - 1, levh - 1, 0, levw, level);
    exline(ammo[p].xp, ammo[p].yp, ammo[p].oxp, ammo[p].oyp, 0, 0, levw - 1, levh - 1, 0xff, levw, mask);

    exspriterep(ammo[p].xp - bullet[ammo[p].type].w / 2,
                ammo[p].yp - bullet[ammo[p].type].h / 2,
                bullet[ammo[p].type].w,
                bullet[ammo[p].type].h, 0, 0,
                levw - 1, levh - 1, levw, 0, level, bullet[ammo[p].type].pic);

    exspriterep(ammo[p].xp - bullet[ammo[p].type].w / 2,
                ammo[p].yp - bullet[ammo[p].type].h / 2,
                bullet[ammo[p].type].w,
                bullet[ammo[p].type].h, 0, 0,
                levw - 1, levh - 1, levw, 0xff, mask, bullet[ammo[p].type].pic);

    ammo[p].active = 0;
}

static void ammoHitToaster(int p, int ox, int oy)
{

    exline(ammo[p].xp, ammo[p].yp, ammo[p].oxp, ammo[p].oyp, 0, 0, levw - 1, levh - 1, 0, levw, level);
    exline(ammo[p].xp, ammo[p].yp, ammo[p].oxp, ammo[p].oyp, 0, 0, levw - 1, levh - 1, 0xff, levw, mask);

    exspriterep(ammo[p].xp - bullet[ammo[p].type].w / 2, ammo[p].yp - bullet[ammo[p].type].h / 2,
                bullet[ammo[p].type].w, bullet[ammo[p].type].h, 0, 0,
                levw - 1, levh - 1, levw, 0, level, bullet[ammo[p].type].pic);

    exspriterep(ammo[p].xp - bullet[ammo[p].type].w / 2, ammo[p].yp - bullet[ammo[p].type].h / 2,
                bullet[ammo[p].type].w, bullet[ammo[p].type].h, 0, 0,
                levw - 1, levh - 1, levw, 0xff, mask, bullet[ammo[p].type].pic);

    ammo[p].active = 0;
}

static void ammoHitPlasma(int p, int ox, int oy)
{

    ammo[p].x = ox;
    ammo[p].y = oy;

    if ((ammo[p].xp < 1) || (ammo[p].yp < 1) || (ammo[p].xp > levw - 2) || (ammo[p].yp >= levh - 2)) {
        ammo[p].active = 0;
    }
}

static void ammoHitMornStar(int p, int ox, int oy)
{

    newammoa(p);

    newboom(ammo[p].xp, ammo[p].yp, 5, 0, 2);

    sound_eff(S_BBLAST, 64, 128, 16384, 1);
    exspriterep(ammo[p].xp - bullet[ammo[p].type].w / 2,
                ammo[p].yp - bullet[ammo[p].type].h / 2,
                bullet[ammo[p].type].w,
                bullet[ammo[p].type].h, 0, 0,
                levw - 1, levh - 1, levw, 0, level, bullet[ammo[p].type].pic);

    exspriterep(ammo[p].xp - bullet[ammo[p].type].w / 2,
                ammo[p].yp - bullet[ammo[p].type].h / 2,
                bullet[ammo[p].type].w,
                bullet[ammo[p].type].h, 0, 0,
                levw - 1, levh - 1, levw, 0xff, mask, bullet[ammo[p].type].pic);

    ammo[p].active = 0;
}

static void ammoHitBlastGun(int p, int ox, int oy)
{
    if (ammo[p].type == W_BLASTGUN) {
        sound_eff(S_BBLGUN, 48, 128, 16384, 0);
    }
    if (ammo[p].type == W_MINIBLAST) {
        sound_eff(S_BMINIB, 48, 128, 16384, 0);
    }
    newboom(ammo[p].xp, ammo[p].yp, 0, 0, 2);
    ammo[p].active = 0;
}

static void ammoHitBlaster(int p, int ox, int oy)
{
    if ((ammo[p].type == W_BLASTER) || (ammo[p].type == W_LASH)) {
        sound_eff(S_BBLAST, 64, 128, 16384, 0);
    }
    if (ammo[p].type == W_BIGBLASTR) {
        sound_eff(S_BBIGBL, 64, 128, 16384, 1);
    }
    newboom(ammo[p].xp, ammo[p].yp, 1, 0, 2);
    ammo[p].active = 0;
}

static void ammoHitMine(int p, int ox, int oy)
{
    sound_eff(S_BGASBL, 64, 128, 16384, 1);
    newboom(ammo[p].xp, ammo[p].yp, 2, 0, 2);
    ammo[p].active = 0;
}

static void ammoHitMegaBlast(int p, int ox, int oy)
{
    sound_eff(S_BMEGAB, 64, 128, 16384, 1);
    newboom(ammo[p].xp, ammo[p].yp, 3, 0, 2);
    ammo[p].active = 0;
}

static void ammoHitMissile(int p, int ox, int oy)
{
    sound_eff(S_BMINIB, 64, 128, 16384, 1);
    newboom(ammo[p].xp, ammo[p].yp, 4, 0, 2);
    ammo[p].active = 0;
}

static void ammoHitBomb(int p, int ox, int oy)
{
    sound_eff(S_BBOMB, 64, 128, 16384, 1);
    newboom(ammo[p].xp + 10, ammo[p].yp + 10, 4, 0, 2);
    newboom(ammo[p].xp + 10, ammo[p].yp - 10, 4, 0, 2);
    newboom(ammo[p].xp - 10, ammo[p].yp + 10, 4, 0, 2);
    newboom(ammo[p].xp - 10, ammo[p].yp - 10, 4, 0, 2);
    newboom(ammo[p].xp, ammo[p].yp, 2, 0, 3);
    ammo[p].active = 0;
}

static void ammoHitPyro(int p, int ox, int oy)
{
    if (ammo[p].timeleft > 375) {
        ammo[p].timeleft = 375;
        newboom(ammo[p].xp, ammo[p].yp, 0, 0, 2);
        sound_eff(S_BMINIB, 64, 128, 16384, 1);
    }
}

static void ammoHitToxicWast(int p, int ox, int oy)
{
    if (ammo[p].timeleft > 600) {
        ammo[p].timeleft = 600;
        exspriterep(ammo[p].xp - bullet[ammo[p].type].w / 2,
                    ammo[p].yp - bullet[ammo[p].type].h / 2,
                    bullet[ammo[p].type].w,
                    bullet[ammo[p].type].h, 0, 0,
                    levw - 1, levh - 1, levw, 0, level, bullet[ammo[p].type].pic);
        exspriterep(ammo[p].xp - bullet[ammo[p].type].w / 2,
                    ammo[p].yp - bullet[ammo[p].type].h / 2,
                    bullet[ammo[p].type].w,
                    bullet[ammo[p].type].h, 0, 0,
                    levw - 1, levh - 1, levw, 0xff, mask, bullet[ammo[p].type].pic);
    }
}

static void ammoHitLaser(int p, int ox, int oy)
{
    newammocrap(p, 2);
    exline(ammo[p].xp, ammo[p].yp, ammo[p].oxp, ammo[p].oyp, 0, 0, levw - 1, levh - 1, 0, levw, level);
    exline(ammo[p].xp, ammo[p].yp, ammo[p].oxp, ammo[p].oyp, 0, 0, levw - 1, levh - 1, 0xff, levw, mask);
    ammo[p].active = 0;
}

static void ammoHitToxicW2(int p, int ox, int oy)
{
    exline(ammo[p].xp, ammo[p].yp, ammo[p].oxp, ammo[p].oyp, 0, 0, levw - 1, levh - 1, 0, levw, level);
    exline(ammo[p].xp, ammo[p].yp, ammo[p].oxp, ammo[p].oyp, 0, 0, levw - 1, levh - 1, 0xff, levw, mask);
    ammo[p].active = 0;
}

static void pickMain(int p, int ammo)
{
    pickammo[ammo].active = 0;
    plr[p].ammo1 = pickammo[ammo].type;
}

static void pickSpecial(int p, int ammo)
{

    pickammo[ammo].active = 0;

    if (plr[p].activeslot == plr[p].nextslot) {
        plr[p].nextslot = (Uint8)((plr[p].nextslot + 1) % slots);
    }

    plr[p].bullets[plr[p].nextslot] = pickammo[ammo].bullets;
    plr[p].slots[plr[p].nextslot] = pickammo[ammo].type;
    plr[p].nextslot = (Uint8)((plr[p].nextslot + 1) % slots);
}

static void pickNextAmmo(int p, int ammo)
{

    logAccess("pickNextAmmo(%d, %d)\n", p, ammo);

    pickammo[ammo].active = 0;
    plr[p].activeslot = (Uint8)((plr[p].activeslot + 1) % slots);
    plr[p].ammo2 = plr[p].slots[plr[p].activeslot];
}
