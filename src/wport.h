/* $Id: wport.h,v 1.1 2003/04/15 18:15:49 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef WPORT_H_INCLUDED
#define WPORT_H_INCLUDED


#include "SDL.h"
#include "util/util.h"

#define u_fscanf1(fp,format,arg1) { fscanf(fp->fp, format, arg1); fp->pos=ftell(fp->fp)-(fp->fstart); }

#define K_ESC		SDLK_ESCAPE
#define K_F11		SDLK_F11
#define K_F12		SDLK_F12
#define K_UP		SDLK_UP
#define K_LEFT		SDLK_LEFT
#define K_RIGHT		SDLK_RIGHT
#define K_RIGHTSHIFT	SDLK_RSHIFT
#define K_DOWN		SDLK_DOWN
#define K_W		SDLK_w
#define K_A		SDLK_a
#define K_D		SDLK_d
#define K_LEFTSHIFT	SDLK_LSHIFT
#define K_S		SDLK_s
#define K_Y		SDLK_y
#define K_G		SDLK_g
#define K_J		SDLK_j
#define K_F		SDLK_f
#define K_H		SDLK_h
#define K_L		SDLK_l
#define K_M		SDLK_m
#define K_ENTER		SDLK_RETURN
#define K_ENTER2	SDLK_KP_ENTER
#define K_SPACE		SDLK_SPACE
#define K_UP2		SDLK_KP8
#define K_LEFT2		SDLK_KP4
#define K_RIGHT2	SDLK_KP6
#define K_DOWN2		SDLK_KP2

// flips screen and updates keyboard state
extern void update();

extern volatile Uint8 key[SDLK_LAST],waskey[SDLK_LAST];

extern volatile short int cstart[768],cdest[768],ctemp[768];
extern volatile short int realfadecount;

extern SDL_Color palette[256];

extern Uint8 fileexists(char *filename);
extern void jcgetblock(int x,int y,int wid,int hei,
                       int pgwid,Uint8 *sour,Uint8 *dest);
extern void jcblockclip(int xx,int yy,int width,int height,int x1,int y1,
                        int x2,int y2,int pgwid,Uint8 *dest,Uint8 *sour);
extern void jvcblock(int x,int y,int wid,int hei,Uint8 *sour);
extern void jcspriterepclip(int xx,int yy,int width,int height,int x1,int y1,
                            int x2,int y2,int pgwid,Uint8 color,
                            Uint8 *dest,Uint8 *sour);
extern void jcspriteclip(int xx,int yy,int width,int height,int x1,int y1,
                         int x2,int y2,int pgwid,Uint8 *dest,Uint8 *sour);
extern void jcscalespritem(int mx,int my,int wid,int hei,int scwid,int schei,
                           int x1,int y1,int x2,int y2,int pgwid,
                           Uint8 *dest,Uint8 *sour);
extern void jcsprite(int x,int y,int wid,int hei,
                     int pgwid,Uint8 *dest,Uint8 *sour);
extern void jsavestart();
extern void jcleardest();
extern void jinitreal();
extern void jsavedest();
extern void jrealfade(Uint8 from,Uint8 tocl,Uint8 frames);
extern void jrealfade1(Uint8 from,Uint8 tocl,Uint8 frames);
extern void jvrc();
extern void jvcfillbox(int x,int y,int x2,int y2,Uint8 color);
extern void jerror(char *description,int errorcode);
extern void jloadpcxpalpartdest(char *filename,Uint8 from,Uint8 tocl);
extern void jloadpcxpalpart(char *filename,Uint8 from,Uint8 tocl);
extern void jclearpal();
extern void jclearstart();
extern void jpal(int color, int r, int g, int b);
extern void jvcsprite(int x,int y,int wid,int hei,Uint8 *sour);
extern void jvdump(int startofs,int pixels,Uint8 *sour);
extern void jvcblockclip(int xx,int yy,int width,int height,
                         int x1,int y1,int x2,int y2,Uint8 *sour);
extern void jvline(int x1,int y1,int x2,int y2,Uint8 c);
extern void jcscalesprite(int xx,int yy,int wid,int hei,int scwid,int schei,
                          int x1,int y1,int x2,int y2,int pgwid,
                          Uint8 *dest,Uint8 *sour);


#endif /* !WPORT_H_INCLUDED */
