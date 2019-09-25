/* logo.c
 * Copyright 1996,2001,2003 Jetro Lauha.
 *
 * $Id: logo.c,v 1.1 2003/04/15 18:15:49 tonic Exp $
 * $Revision: 1.1 $
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "wport.h"
#include "timer.h"

Uint8 *logotmp;
PIC logopic;

void logo_init()
{
    util_loadpcxpi(LOGOPICFILE, &logopic);
    logotmp = (Uint8 *) malloc(logopic.w * logopic.h);
    if (logotmp == NULL)
	jerror("LOGO.init.malloc: No mem", logopic.w * logopic.h);
}

void logo_deinit()
{
    free(logotmp);
    logotmp = NULL;
    util_freepi(&logopic);
}

void logo(Uint8 * destp, Uint8 * back, int cnt)
{
    int a;
    Uint8 *dest = destp, *sour = logotmp;
    memcpy(logotmp, back, logopic.w * logopic.h);
    jcscalespritem(320, logopic.h / 2, logopic.w, logopic.h,
		   logopic.w +
		   (logopic.h * sint[(cnt * 6) % SCT]) / (FIXS * 4),
		   logopic.h * 3 / 4 +
		   (logopic.h * sint[(cnt * 6 + SCT / 2) % SCT]) / (FIXS *
								    4), 0,
		   0, 639, logopic.h - 1, 640, logotmp, logopic.pic);
    for (a = 0; a < logopic.w * logopic.h; a += 4)
    {
	*(unsigned long *) (dest) = (((*(unsigned long *) (dest) << 1) +
				      *(unsigned long *) (dest) +
				      *(unsigned long *) (sour)) >> 2) & 0x3f3f3f3f;
	dest += 4;
	sour += 4;
    }
}
