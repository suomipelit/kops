/**
 * Header util.c
 *
 * $Id: util.h,v 1.1 2003/04/15 18:15:56 tonic Exp $
 * $Revision: 1.1 $
 *
 * Copyright 2002 Sami Makinen-Okubo. All Rights Reserved.
 *
 * Author: Sami Makinen-Okubo
 *
 * Description:
 * 
 *   Header file for util.c.
 */

#ifndef KOPS_BOT_UTIL
#define KOPS_BOT_UTIL 1

#include <SDL_stdinc.h>
#include "bot-config.h"

#ifndef TRUE
#  define TRUE 1
#endif
#ifndef FALSE
#  define FALSE 0
#endif

#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS  0
#  define EXIT_FAILURE  1
#endif

/**
 * Some bit field manipulation macros.
 */
#define SET_FLAGS(FLAGS, DST) (DST |= FLAGS)
#define CLR_FLAGS(FLAGS, DST) (DST ^=  FLAGS)
#define IS_SET(FLAGS, SRC) ((FLAGS & SRC) == FLAGS)
#define IS_CLR(FLAGS, SRC) ((FLAGS & SRC) == 0)

/**
 * Define bcopy(2) in portable way.
 */
#if HAVE_MEMCPY
#  define BCOPY(SRC, DEST, LEN) memcpy(DEST, SRC, LEN)
#elif HAVE_BCOPY
#  define BCOPY(SRC, DEST, LEN) bcopy(SRC, DEST, LEN)
#else
#  error "Required memcpy or bcopy is missing. Configuration problem?"
#endif

/**
 * Define bzero(2) in portable way.
 */
#if HAVE_MEMSET
#  define BZERO(SRC, LEN)  memset(SRC, 0, LEN)
#elif HAVE_BZERO
#  define BZERO(SRC, LEN)  bzero(SRC, LEN)
#else
#  error "Required memset or bzero is missing. Configuration problem?"
#endif

/**
 * Define stricmp(3) in portable way.
 */
#if HAVE_STRICMP
#  define STRICMP(STR1, STR2) stricmp(STR1, STR2)
#elif HAVE_STRCASECMP
#  define STRICMP(STR1, STR2) strcasecmp(STR1, STR2)
#else
#  error "Required stricmp or strcasecmp is missing. Configuration problem?"
#endif

/**
 * Debugging levels.
 */

#define DBG_MAI  1    /* Debug main module (cannonfodder.c). */
#define DBG_UTI  2    /* Debug util.c module. */
#define DBG_BOT  4    /* Debug generated bot modules. */
#define DBG_SPC  8    /* Debug special-action.c module. */
#define DBG_PIO  16   /* Debug parser-io.c module. */
#define DBG_GLO  32   /* Debug globally visible (non-static) functions. */
#define DBG_STA  64   /* Debug module visible (static) functions. */
#define DBG_FUN  128  /* Debug function entering. */
#define DBG_RET  256  /* Debug function returning. */
#define DBG_DET  512  /* Debug miscellaneous details. */
#define DBG_RAD  1024 /* Debug radar. */
#define DBG_KB   2048 /* Debug radar. */

/**
 * logDebug(LVL, ARGS...) - a macro for debug logging. 
 *
 * Macro uses debugMsg() -function for actual debug output but
 * the entry point should be the logDebug -macro which sets the format
 * and adds some additional information of the debug location.
 * The macro is expanded only if DEBUG is defined.
 * The first argument to macro is a level of details the 
 * debug message will reveal. At run-time the level be checked
 * against the current user wanted debug details and only, if
 * the user wanted level has same bit set as the message has,
 * the message will be printed.
 * The second argument is format as in printf(3) -function.
 * And the rest of the arguments are converted for output
 * according to given format.
 */
#ifdef _WIN32

#ifdef DEBUG
/* not as good as the *nix version */
#define logDebug debugMsg
#else
static void _debug_kludge(int, ...) {}
#define logDebug _debug_kludge
#endif /* !DEBUG */

#else /* or !_WIN32 */

#ifdef DEBUG
#define debugfmt(FMT, ARGS...) "%-23.23s(%3.3d): " FMT "\n", __FILE__ , __LINE__ , ## ARGS
#define logDebug(LVL, ARGS...) debugMsg(LVL, debugfmt(ARGS))
#else
#define logDebug(LVL, ARGS...) 
#endif /* !DEBUG */

#endif /* !_WIN32 */

/**
 * Print out debug message.
 * Used by logDebug -macro. 
 */ 
extern void debugMsg(Uint32 lvl, char* fmt, ...);

/**
 * Log access, error or warning.
 * 
 */ 
extern void logAccess(char* fmt, ...);
extern void logError(char* fmt, ...);
extern void logWarning(char* fmt, ...);

extern void* safeMalloc(size_t bytes);
extern void  str2upper(char* str);

struct KNOWLEDGE;
struct RULE;
struct PERCEPT;
extern void logKBGrid(const struct KNOWLEDGE* kb);
extern void logRule(const struct KNOWLEDGE* kb, const struct RULE* );
extern void logPercept(const struct KNOWLEDGE* kb, const struct PERCEPT* );

/**
 * Check if the system has perror, if not, define new.
 */
#if !HAVE_PERROR
#  define perror(ARGS...) fprintf(stderr, ## ARGS)
#endif

#endif /* !KOPS_BOT_UTIL */

/* EOF util.h $Id: util.h,v 1.1 2003/04/15 18:15:56 tonic Exp $ */


