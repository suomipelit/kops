/* $Id: parser-io.h,v 1.1 2003/04/15 18:15:56 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef KOPS_BOT_PARSER_IO_H
#define KOPS_BOT_PARSER_IO_H 1

#include <stdio.h>
#include "kb-parser.h"

#define INDENT_WIDTH 4

#define BUFFER_SIZE       1024
#define INIT_PREFIX       "initKB_"
#define VAR_TABLE_PREFIX  "variables_"

#define SCAN_MODE 0
#define PEEK_MODE 1

enum { 
    token_start = 0,  /*  0 */
    token_kb_name,    /*  1 */
    token_var_def,    /*  2 */
    token_open_rule,  /*  3 */
    token_close_rule, /*  4 */
    token_name,       /*  5 */
    token_number,     /*  6 */
    token_not,        /*  7 */
    token_and,        /*  8 */
    token_implies,    /*  9 */
    token_assign,     /* 10 */
    token_eof,        /* 11 */
    token_cmd,        /* 12 */
    token_label,      /* 13 */
    token_unknown     /* 14 */
};

typedef struct KBInfo {
    char* name;
    struct KBInfo* next;
} KBInfo;

typedef struct IOContext {
    FILE* h_file;
    FILE* c_file;

    KBInfo* available;
    int     available_count;
} IOContext;

typedef struct Tokenizer {
    
    FILE *input;
    
    char  buffer[BUFFER_SIZE];
    int   cursor;
    int   lineNumber;

    char  str1[BUFFER_SIZE];
    char  str2[BUFFER_SIZE];
    char* currentStr;
    char* nextStr;
    
    int   currentNumber;
    int   nextNumber;
    int   currentToken;
    int   nextToken;
    int   lastToken;

    int   peeked;

} Tokenizer; 

extern int  initIOContext(IOContext* io_ctx, KBContext* kb_ctx);
extern void finalizeIOContext( IOContext* io_ctx );
extern int  printKBFiles(IOContext* io_ctx, KBContext *kb_ctx);
extern int  loadKBRules(KBContext *kb_ctx, char *filename);

#endif /* !KOPS_BOT_PARSER_IO_H */

/* EOF parser-io.h $Id: parser-io.h,v 1.1 2003/04/15 18:15:56 tonic Exp $ */
