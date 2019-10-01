/* utilpic.h
 *
 * $Id: utilpic.h,v 1.1 2003/04/15 18:15:57 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef UTILPIC_H_INCLUDED
#define UTILPIC_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#include <SDL_stdinc.h>

typedef struct
{
    int w, h;
    Uint8 *pic;
    Uint8 *pal;
    int bitspp, bytespp;
} PIC;

#ifdef __cplusplus
};
#endif
#endif
