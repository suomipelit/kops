/* $Id: global.h,v 1.1 2003/04/15 18:15:49 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef KOPS_GLOBAL_H
#define KOPS_GLOBAL_H 1

#include "SDL.h"
#include "util/util.h"

#define REG

#if !defined(_WIN32) && !defined(__MSDOS__)
#undef stricmp
#define stricmp strcasecmp
#endif

/*********  general constants  ***********************************/
#define X_RESOLUTION 640
#define Y_RESOLUTION 480

#define FIXP 10 /* .x fixed point */
#define FIXS (1 << FIXP)
#define STARTENERGY 100
#define CHARGERATE (FIXS / 24)
#define BORHIT 3 /* border hit multiplier */
#define GNDHIT 2 /* ground hit multiplier */
#define FRICTION 199 / 200
#define ACCELDIV 23 /* acceleration divider */
/* #define TURNRATE    (FIXS*17/30) */
#define TURNRATE (FIXS * 2 / 3)
#define MAXSPEED (FIXS * 29 / 10)
#define BOUNCEDIV 4   /* speed divider for bouncing */
#define REGENRATE 4   /* base regeneration rate */
#define WTTRANSLEN 13 /* water transparency wave length */
#define WTTABLELEN 20 /* water transparency addtable length */
#define MAXCRAP 256   /* max crap (ex waterdrops/engine flame) */
#define MAXAMMO 1024
#define MAXBOOMS 32
#define PICKDIST 11
#define PICKRATE 600
#define SHIPHITDIST 11
#define TIMERRATE 66 /* timer update rate per second */
#define MAXPLAYERS 4
#define MAXBOTS 4      /* Max. bots. Added by SMO. */
#define BOOMTYPES 6    /* explosion types */
#define BULLETTYPES 40 /* drawn bullet types      : these should */
#define BULLETBOXTS 40 /* drawn boxes for bullets : be same      */
#define OTHERGFX 3     /* other gfx */
#define WINW 320       /* player window width */
#define WINH 240       /*      ""       height */
#define SHIPW 32
#define SHIPH 32
#define SHIPSIZE (SHIPW * SHIPH)
#define SHIPFRM 64
#define SHIPTOTSIZE (SHIPFRM * SHIPSIZE)
#define SCT 1024 /* sin & cos table size */
#define FRFIX (SCT / SHIPFRM)
#define MAXSLOTS 16
#define FONTCHARS 69
#define BIGFONTCHRS 71
#define MAXLEVELS 1024
#define MAXGMUS 256
#define ALLSAMPLES 262144
// SP: string limits
#define WEAPON_NAME_LENGTH 21
#define LEVEL_NAME_LENGTH 14
#define EFFECT_NAME_LENGTH 13
/*********  debug/test/etc defines  ************************/
/* #define ONEPLRTWEAK */
/*********  file names  ************************************/
#define KOPSJFLFILE "kops.jfl"
#define KOPSCFGFILE "kops.cfg"
#define FILLERPICFILE "fillshp.pcx"
#define MENUBACKPICFILE "shipembs.pcx"
#define LOGOPICFILE "kopslogo.pcx"
#define KOPSGFXFILE "kops.gfx"
#define KOPSSHIPFILE "kops.shp"
#define BIGFONTFILE "bigfont.pcx"
#define MINITFILE "tense.ini"
#define MUSICFILE "tense.s3m"
//#define MINITFILE       "_kops.mus"
//#define MUSICFILE       "_kops.s3m"
#define TRMPDRIVERFILE "trmpx.drv"
/*********  level filenames etc  ***************************/
#ifdef REG
#define LEVELS 9
#else
#ifdef ONEDISK
#define LEVELS 4
#else
#define LEVELS 5
#endif
#endif

/* Number of main weapons. Added by SMO. */
#define W_MAIN_MAX 9

/*********  weapons  (9/29) ********************************/
#define W_TUHNU 0
#define W_PEAGUN 1
#define W_SLING 2
#define W_BIGPEAGUN 3
#define W_BIGSLING 4
#define W_BLASTGUN 5
#define W_MINIBLAST 6
#define W_BLASTER 7
#define W_BIGBLASTR 8
/*  special weapons etc start here */
#define W_GASBLAST 9
#define W_MEGABLAST 10
#define W_MINE 11
#define W_HEATSEEK 12
#define W_MORNSTAR 13
#define W_SENSORJAM 14
#define W_NOWEAPON 15
#define W_PYROTECH 16
#define W_TOXICWAST 17
#define W_TOASTER 18
#define W_REARSLING 19
#define W_RINGFIRE 20
#define W_CRAWLER 21
#define W_GRAVFLOW 22
#define W_BOMB 23
#define W_LASH 24
#define W_ULTRATHR 25
#define W_RETRO 26
#define W_HYPERSPAC 27
#define W_PLASMA 28
#define W_PULSLASER 29
#define W_DELBLAST 30
#define W_FLIPPER 31
#define W_SIGNAL 32
#define W_NEXTAMMO 33
#define W_TUHNRUISK 34
#define W_HYPERBLST 35
#define W_TRACTBEAM 36
#define W_TWINLASER 37
#define W_SUPERPYRO 38
#define W_SPILLER 39
#define W_TOXICW2 40
#define W_DOT 41
#define W_TOTAL 42 /* Added by SMO */

/*********  sound effects  ********************************************/
#define SNDEFFECTS 49
#define S_TUHNU 0
#define S_PEAGUN 1
#define S_SLING 2
#define S_BIGPEAGUN 3
#define S_BIGSLING 4
#define S_BLASTGUN 5
#define S_MINIBLAST 6
#define S_BLASTER 7
#define S_BIGBLASTR 8
#define S_GASBLAST 9
#define S_MEGABLAST 10
#define S_MINE 11
#define S_HEATSEEK 12
#define S_HEATSTIK 13
#define S_MORNSTAR 14
#define S_SENSORJAM 15
#define S_PYROTECH 16
#define S_TOXICWAST 17
#define S_TOASTER 18
#define S_REARSLING 19
#define S_RINGFIRE 20
#define S_CRAWLER 21
#define S_GRAVFLOW 22
#define S_BOMB 23
#define S_LASH 24
#define S_LASHTIK 25
#define S_ULTRATHR 26
#define S_RETRO 27
#define S_HYPERSPAC 28
#define S_PLASMA 29
#define S_PULSLASER 30
#define S_DELBLAST 31
#define S_FLIPPER 32
#define S_SIGNAL 33
#define S_COLLECT 34
#define S_MENUMOVE 35
#define S_MENUCHOOS 36
#define S_MENUCHANG 37
#define S_BMINIB 38
#define S_BBIGBL 39
#define S_BBLAST 40
#define S_BBLGUN 41
#define S_BBOMB 42
#define S_BDELBL 43
#define S_BGASBL 44
#define S_BMEGAB 45
#define S_BSHIP 46
#define S_SHIPHIT 47
#define S_GNDHIT 48

/*********  active stuff  *********************************************/

typedef struct PLAYER {

    int bot;       /* true, if controlled by an agent. Added by SMO. */
    int action;    /* agent decided action, like plr key control. Added by SMO. */
    int fire_rate; /* control how often bot can shoot with main. */

    Uint8 active;
    Uint8 *ship;                                 /* ship image data */
    Uint8 *scr, *tmp;                            /* temp screen mem */
    int energy;                                  /* fixed point */
    int x, y;                                    /* x,y (fixed point) */
    int xi, yi;                                  /* velocity (fixed point) */
    int dir;                                     /* direction */
    int xp, yp;                                  /* converted from x,y to pixel coords */
    int wx, wy;                                  /* window upper left corner coordinates */
    Uint8 ammo1, ammo2;                          /* ammo types */
    Uint8 fireflag;                              /* ammo1 fireflag */
    int firepause;                               /* ammo2 firepause */
    Uint8 slots[MAXSLOTS];                       /* weapon slots */
    int bullets[MAXSLOTS];                       /* weapon bulletcounters for slots */
    Uint8 activeslot;                            /* active weapon slot */
    Uint8 nextslot;                              /* next slot for pickammo */
    int slotchangepause;                         /* slot change pause */
    Uint8 controltype;                           /* keyboard=0, mouse=1, joystick=2 */
    int k_up, k_left, k_right, k_fire1, k_fire2; /* keys */
    Uint8 blink;                                 /* damage blinker */
    Uint8 ground;                                /* ground flag */
    Uint8 water;                                 /* water flag */
    int sensorjam;                               /* sensorjam counter */
    Uint8 hyperspac;                             /* hyperspace projector flag */
    int flip, signal;                            /* flip/signalblur counter */
} PLAYER;

typedef struct AMMO {
    Uint8 active;
    Uint8 type;
    int hitdist;
    int damage; /* fixed point */
    int timeleft;
    int x, y, xi, yi; /* fixed point */
    int xp, yp, oxp, oyp;
} AMMO;

typedef struct CRAP {
    Uint8 active;
    Uint8 type;
    int hitdist;
    int timeleft;
    int x, y, xi, yi; /* fixed point */
    int xp, yp;
    Uint8 c;
} CRAP;

typedef struct BOOM {
    Uint8 active;
    Uint8 type, transp;
    int xp, yp; /* center */
    Uint8 frm, speed;
} BOOM;

typedef struct WATER {
    short int x, y;
} WATER;

typedef struct BASE {
    short int x, y;
    Uint8 c;
} BASE;

typedef struct PICKAMMO {
    Uint8 active;
    Uint8 type;
    int timeleft;
    int bullets;
    int x, y, orgy; /* fixed point */
    int xp, yp, cnt;
} PICKAMMO;

/*********  gfx image structures  *************************************/

typedef struct BOOMANIM {
    int w, h, siz;
    int frames;
    Uint8 *pic;
} BOOMANIM;

typedef struct BULLET {
    int w, h, siz, frames;
    Uint8 *pic; /* points to "gfx" */
} BULLET;

typedef struct BULLETBOX {
    int w, h, siz, prob;
    Uint8 *pic; /* points to "gfx" */
} BULLETBOX;

typedef struct OTHGFX { /* "other gfx" */
    int w, h, frames, siz;
    Uint8 *pic; /* points to "gfx" */
} OTHGFX;

typedef struct FONT {
    Uint8 w, h;
    Uint8 *o[256]; /* pointers for letters to p */
    Uint8 *p;      /* letters */
} FONT;

/* Added by SMO. */
typedef struct WEAPON_INFO {

    int firepause; /* Pause after fireing. */
    float recoil;  /* recoil modifier */

    /* Sound effect. */
    int sampleno;
    int volume;
    int freq;
    int chgroup;

    void (*special)(int);           /* special handler */
    void (*move)(int);              /* ammo moving handler */
    void (*hit)(int, int);          /* ammo hit player handler */
    void (*ammohit)(int, int, int); /* ammo hit ground handler */
    void (*pick)(int, int);
} WEAPON_INFO;

#if 0 /* Not used yet. */
typedef struct BASE_CLUSTER {
    int                  x;    /* average of base cluster pixel x-coordinates */
    int                  y;    /* average of base cluster pixel y-coordinates */
    struct BASE_CLUSTER *next; /* form a linked list of all base clusters     */
} BASE_CLUSTER;
#endif

/* End SMO */

extern volatile unsigned long int framecounter;
extern volatile Uint8 gamepause;
extern Uint32 cnt;
extern char *copyright;
extern int windowed;
extern int versmaj, versmin;                          /* major & minor version */
extern const char *sp_version_postfix;                /* Suomipelit port version postfix */
extern Uint8 *level, *backgr, *mask;                  /* level & background image and mask */
extern Uint8 parallaxdraw;                            /* parallax on/off */
extern int levw, levh, backw, backh;                  /* level+background width & height */
extern int sint[SCT], cost[SCT];                      /* sin & cos tables */
extern Uint8 *gfx;                                    /* gfx (bullets, boxes etc) */
extern Uint8 *transp;                                 /* transparency table */
extern WATER *water;                                  /* water wave coords */
extern BASE *base;                                    /* bases */
extern PICKAMMO *pickammo;                            /* pickammos */
extern int *wave;                                     /* moving waterwave */
extern Uint8 slots;                                   /* max amount of active weapon slots */
extern int waterwaves, basepixels;                    /* waterwaves & basepixels on level */
extern int basepixcount;                              /* base regeneration counter */
extern char wnames[BULLETTYPES][WEAPON_NAME_LENGTH];  /* weapon names */
extern int pickammospots;                             /* pickammo spots */
extern int totalprob;                                 /* ammo probabilities */
extern int alllevels, levels;                         /* amount of all&choosed levels */
extern char levall[MAXLEVELS][LEVEL_NAME_LENGTH];     /* all levels found */
extern char levlist[MAXLEVELS][LEVEL_NAME_LENGTH];    /* choosed levels */
extern char effnames[SNDEFFECTS][EFFECT_NAME_LENGTH]; /* sound effect file names */
extern Uint8 m_game[MAXGMUS];                         /* music starting orders info */
extern int m_startmenu, m_status, m_endgame, m_info;
extern int mgcount;     /* count of game musics */
extern char *minitfile; /* music init file */
extern Uint8 s_sdevice, s_interp, s_shortchn, s_longchn, s_musvol, s_effvol;
extern Uint32 s_samprate;              /* sound infos */
extern Uint8 snd;                      /* sounds on/off (1/0) */
extern Uint8 kludgesnd;                /* force sound disable as an command line parameter */
extern int wins[MAXPLAYERS + MAXBOTS]; /* win table */
extern int round_number;               /* round number */
extern int totalframes, drawnframes;   /* for average FPS */
extern Uint8 gamewindowcolorrand;
extern Uint8 players;
extern Uint8 bots;
extern Uint8 testmode;
extern Uint32 nextammo, nextcrap, nextboom;
extern Uint8 wttrans[WTTRANSLEN];
extern Uint8 wttable[WTTABLELEN];
extern PLAYER plr[MAXPLAYERS + MAXBOTS];
extern CRAP crap[MAXCRAP];
extern AMMO ammo[MAXAMMO];
extern BOOM boom[MAXBOOMS];
extern BOOMANIM boomanim[BOOMTYPES];
extern BULLET bullet[BULLETTYPES];
extern BULLETBOX bulletbox[BULLETBOXTS];
extern OTHGFX othgfx[OTHERGFX];
extern FONT font, bigfont[2];
extern Uint8 gravvalue;
extern int gravity;
extern int watergrav;
extern int ammogravity;

extern SDL_Surface *screen;
extern SDL_Surface *windowSurface;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;

/* Added by SMO */
extern WEAPON_INFO weapons[];
extern void initweapons();
/* End SMO */

extern int isqrt(int num);

#endif /* !KOPS_GLOBAL_H */
