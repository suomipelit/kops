/**
 * Module util.c $Revision: 1.1 $
 *
 * Copyright 2002 Sami Makinen-Okubo. All Rights Reserved.
 *
 * Author: Sami Makinen-Okubo
 *
 * Description:
 * 
 *   Some general purpose utility routines.
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

#include "bot-util.h"
#include "cannonfodder.h"

/**
 * Print out debug message.
 * Used by logDebug -macro (see util.h).
 *
 * @param lvl - the level when debug message will be shown.
 * @param fmt - format as in printf(3) call.
 * @param ... - variable arguments as in printf(3) call.
 */ 
void
debugMsg(Uint32 lvl, char* fmt, ...) {

  va_list ap;

  if((lvl & config.debug_lvl) == lvl) {

    va_start(ap, fmt);
    vfprintf(config.debug_log, fmt, ap);
    va_end(ap);
  }
}

/**
 * Log access or activity.
 * Writes log message to access log defined in global configuration.
 *
 * @param fmt - format as in printf(3) call.
 * @param ... - variable arguments as in printf(3) call.
 */
void
logAccess(char* fmt, ...) {

  time_t now;
  char date_buf[30];
  va_list ap;

  time(&now);
  strftime(date_buf, 30, "%a %Y/%m/%d %T" , localtime(&now));
  fprintf(config.access_log, "%s: ", date_buf);

  va_start(ap, fmt);
  vfprintf(config.access_log, fmt, ap);
  va_end(ap);

  fprintf(config.access_log, "\n");
}

/**
 * Log error.
 * Writes error message to error log defined in global configuration.
 *
 * @param fmt - format as in printf(3) call.
 * @param ... - variable arguments as in printf(3) call.
 */
void
logError(char* fmt, ...) {

  time_t now;
  char date_buf[30];
  va_list ap;

  time(&now);
  strftime(date_buf, 30, "%a %Y/%m/%d %H:%M:%S" , localtime(&now));
  fprintf(config.error_log, "%s: ", date_buf);

  va_start(ap, fmt);
  vfprintf(config.error_log, fmt, ap);
  va_end(ap);

  fprintf(config.error_log, "\n");
}

/**
 * Log warning.
 * Writes warning message to error log defined in global configuration.
 *
 * @param fmt - format as in printf(3) call.
 * @param ... - variable arguments as in printf(3) call.
 */
void
logWarning(char* fmt, ...) {

  time_t now;
  char date_buf[30];
  va_list ap;

  time(&now);
  strftime(date_buf, 30, "%a %Y/%m/%d %T" , localtime(&now));
  fprintf(config.error_log, "%s: ", date_buf);

  va_start(ap, fmt);
  vfprintf(config.error_log, fmt, ap);
  va_end(ap);
  fprintf(config.error_log, "\n");
}

/**
 * Malloc with error check.
 * Allocates memory for and if fails, prints error message and exits.
 *
 * @param bytes - size of wanted memory block.
 *
 * @return pointer to an allocated memory.
 */
void*
safeMalloc(size_t bytes) {

  void* space;

  space = malloc(bytes);

  if( !space ) {
    logError("Ran out of memory. Exiting.");
    exit( EXIT_FAILURE );
  }

  return space;
}

/**
 * Convert string to upper case.
 */
void
str2upper(char* str) {

    assert( str != NULL);

    while( *str != '\0' ) {
	*str = toupper( *str );
	++str;
    }
}

void
logKBGrid(const KNOWLEDGE* kb) {

    int  x, y;
    int  pos;
    char mark;

    fprintf(config.access_log, "\n");
    for(y = 0; y < kb->grid_height; ++y ) {
	for(x = 0; x < kb->grid_width; ++x ) {

	    pos = y * kb->grid_width + x;
	    mark = '!';

	    if ( IS_SET( SQUARE_BASE, kb->grid[pos] )) {
		mark = '*';
	    }
	    else if ( IS_SET( SQUARE_GROUND, kb->grid[pos] )) {
		mark = '#';
	    }
	    else if ( IS_SET( SQUARE_WATER, kb->grid[pos] )) {
		mark = '~';
	    }
	    else if( IS_SET(SQUARE_EMPTY, kb->grid[pos]) ) {
		mark = ' ';
	    }
	    else if ( SQUARE_UNKNOWN == kb->grid[pos] ) {
		mark = '?';
	    }

	    fprintf(config.access_log, "%c", mark);
	}
	fprintf(config.access_log, "\n");
    }
    fprintf(config.access_log, "\n");
}

static void
logFlagNames(FILE* output, const NAME* table, int max, flag_t value) {
    int index;
    int not_first;

    not_first = 0;
    for(index = 0; index < max; ++index) {

	if (IS_SET( (unsigned)table[index].key, value )) {
	    if( not_first ) {
		fprintf(output, "|");
	    }
	    fprintf(output, "%s", table[index].name);
	    not_first = 1;
	}
    }
}

void
logPercept(const KNOWLEDGE* kb, const PERCEPT* percept) {
    fprintf(config.access_log, "PERCEPT: ");
    logFlagNames(config.access_log, kb->percept_names, kb->percept_count, percept->flags );
    fprintf(config.access_log, "\n");
}

void
logRule(const KNOWLEDGE* kb, const RULE* rule) {
    fprintf(config.access_log, "RULE:\n");

    fprintf(config.access_log, "premise positive variables: ");
    logFlagNames(config.access_log, kb->variable_names, kb->variable_count, rule->premise_positive_variables);

    fprintf(config.access_log, "\npremise negative variables: ");
    logFlagNames(config.access_log, kb->variable_names, kb->variable_count, rule->premise_negative_variables);

    fprintf(config.access_log, "\npremise positive percepts: ");
    logFlagNames(config.access_log, kb->percept_names, kb->percept_count, rule->premise_positive_percepts);

    fprintf(config.access_log, "\npremise negative percepts: "); 
    logFlagNames(config.access_log, kb->percept_names, kb->percept_count, rule->premise_negative_percepts);

    fprintf(config.access_log, "\nconclusion positive variables:  ");
    logFlagNames(config.access_log, kb->variable_names, kb->variable_count, rule->conclusion_positive_variables);

    fprintf(config.access_log, "\nconclusion negative variables:  ");
    logFlagNames(config.access_log, kb->variable_names, kb->variable_count, rule->conclusion_negative_variables);

    fprintf(config.access_log, "\nspecial action:                %s\n",  (rule->special == NULL?"no":"yes") );

    fprintf(config.access_log, "conclusion action:             %#x\n", rule->conclusion_action.flags);
}

/* EOF util.c $Id: util.c,v 1.1 2003/04/15 18:15:56 tonic Exp $ */




