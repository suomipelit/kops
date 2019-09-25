/* $Id: gamefunc.h,v 1.1 2003/04/15 18:15:49 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef GAMEFUNC_H_INCLUDED
#define GAMEFUNC_H_INCLUDED


extern void placeplayer(int nro);

extern Uint8 hitdetect(int x, int y);
extern Uint8 waterdetect(int x, int y);
extern Uint8 basedetect(int x, int y);


extern void newammo(int player, int type);
extern void newammoa(int nro);
extern void newplrcrap(int player, int type);
extern void newammocrap(int nro, int type);
extern void newpickammo();
extern void newcrap(int xp, int yp, int type);
extern void newboom(int xp, int yp, int type, int transp, int speed);
extern void savescreenrix(char *fname);


#endif /* !GAMEFUNC_H_INCLUDED */
