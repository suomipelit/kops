/* $Id: config.h,v 1.1 2003/04/15 18:15:55 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef KOPS_BOT_CONFIG_H
#define KOPS_BOT_CONFIG_H 1

#ifndef INIT_C_FILE
#define INIT_C_FILE "kb-init.c"
#endif

#ifndef INIT_H_FILE
#define INIT_H_FILE "kb-init.h"
#endif

#define DEBUG 1

#define HAVE_STDARG_H 1
#define HAVE_ASSERT_H 1
#define STDC_HEADERS 1
#define HAVE_ERRNO_H 1
#define HAVE_TIME_H 1
#define HAVE_CTYPE_H 1
#undef HAVE_SYS_TIME_H
#undef HAVE_STRINGS_H

#define HAVE_MEMCPY 1
#define HAVE_MEMSET 1
#ifdef _WIN32
#define HAVE_STRICMP 1
#else
#define HAVE_STRCASECMP 1
#endif
#define HAVE_PERROR 1

#if STDC_HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#elif HAVE_STRINGS_H
#include <strings.h>
#endif

#if HAVE_ASSERT_H
#include <assert.h>
#endif

#if HAVE_ERRNO_H
#include <errno.h>
#elif !defined(errno)
extern int errno;
#endif

#if HAVE_STDARG_H
#include <stdarg.h>
#endif

#if HAVE_TIME_H
#include <time.h>
#elif HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#error "time.h or sys/time.h required."
#endif

#if HAVE_CTYPE_H
#include <ctype.h>
#endif

typedef struct config {
    int debug_lvl;
    FILE *debug_log;
    FILE *access_log;
    FILE *error_log;
} Config;

/**
 * The log file locations and desired debug level 
 * are stored in config.
 */
extern Config config;

#endif /* !KOPS_BOT_CONFIG_H */

/* EOF config.h $Id: config.h,v 1.1 2003/04/15 18:15:55 tonic Exp $ */
