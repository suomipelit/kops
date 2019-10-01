/* $Id: logo.h,v 1.1 2003/04/15 18:15:49 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef LOGO_H_INCLUDED
#define LOGO_H_INCLUDED

#include <SDL_stdinc.h>

extern void logo_init();
extern void logo_deinit();
extern void logo(Uint8 *destp, Uint8 *back, int cnt);

#endif /* !LOGO_H_INCLUDED */
