/* $Id: load.h,v 1.1 2003/04/15 18:15:49 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef LOAD_H_INCLUDED
#define LOAD_H_INCLUDED


extern void saveconfig();
extern void newconfig();
extern Uint8 loadconfig();
extern void loadlevel(char *filename);
extern void loadships(char *filename);
extern void loadgfx(char *filename);


#endif /* !LOAD_H_INCLUDED */
