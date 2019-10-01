/* $Id: init.h,v 1.1 2003/04/15 18:15:49 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

extern char musicfile[80];

extern void init();
extern void deinit();
extern void initplayer(int nro);
extern void resetplayer(int nro);
extern void resetplayer2(int nro);
extern void deinitplayer(int nro);
extern void initgravity();

#endif /* !INIT_H_INCLUDED */
