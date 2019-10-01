/* $Id: spr.h,v 1.1 2003/04/15 18:15:49 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef SPR_H_INCLUDED
#define SPR_H_INCLUDED

#include <SDL_stdinc.h>
#include "global.h"

extern void kprintf(Uint8 *dest, FONT fnt, int x, int y, int pgwid, char *format, ...);
extern void kprintfs(FONT fnt, int x, int y, char *format, ...);
extern void sctrspritem(int mx, int my, int wid, int hei, int scwid,
                        int schei, int x1, int y1, int x2, int y2, int pgwid,
                        Uint8 *dest, Uint8 *sour);
extern void exspriterep(int xx, int yy, int width, int height, int x1,
                        int y1, int x2, int y2, int pgwid, Uint8 color,
                        Uint8 *dest, Uint8 *sour);
extern void trsprite(int xx, int yy, int width, int height, int x1, int y1,
                     int x2, int y2, int pgwid, Uint8 *dest, Uint8 *sour);
extern void exline(int x1, int y1, int x2, int y2, int cx1, int cy1,
                   int cx2, int cy2, Uint8 c, int pgwid, Uint8 *dest);

#endif /* !SPR_H_INCLUDED */
