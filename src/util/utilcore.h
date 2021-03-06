/* utilcore.h
 *
 * $Id: utilcore.h,v 1.1 2003/04/15 18:15:57 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef UTILCORE_H_INCLUDED
#define UTILCORE_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <SDL_stdinc.h>

#include "utilpic.h"

#ifndef PI
#define PI 3.1415926535897932
#endif
#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif
#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

extern Uint32 util_gettick();
extern void util_line(int x1, int y1, int x2, int y2, Uint8 c, int pgwid, char *dest);
extern void util_lineclip(int x1, int y1, int x2, int y2, int cx1, int cy1,
                          int cx2, int cy2, Uint8 c, int pgwid, char *dest);
extern void util_loadpcx(char *dest, char *filename, Uint32 bytes);
extern void util_loadpcxpi(char *filename, PIC *pic);
extern void util_freepi(PIC *pic);
extern void util_xorrand(char *data, int seed, int datalen);
extern Uint32 util_filesize(FILE *fp);
extern char util_fileexists(char *filename);

#ifdef __cplusplus
};
#endif
#endif
