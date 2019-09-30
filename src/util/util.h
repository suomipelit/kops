/* $Id: util.h,v 1.1 2003/04/15 18:15:57 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef UTIL_MASTER_H_INCLUDED
#define UTIL_MASTER_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <SDL_stdinc.h>
#include "utilpic.h"
#include "utilcore.h"
#include "utilfile.h"

#if !defined(_WIN32) && !defined(__MSDOS__)
#undef stricmp
#define stricmp strcasecmp
#endif

#endif
