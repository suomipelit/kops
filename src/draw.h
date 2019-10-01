/* $Id: draw.h,v 1.1 2003/04/15 18:15:49 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

#include <SDL_stdinc.h>

extern void parallax(Uint8 * dest, int winx, int winy);
extern void updatescreen();
extern void drawscreen();


#endif /* !DRAW_H_INCLUDED */
