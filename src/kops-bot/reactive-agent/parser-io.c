/* parser-io.c
 * Copyright 2002 Sami Mäkinen-Okubo
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

#include "config.h"
#include "kb-parser.h"
#include "parser-io.h"
#include "util.h"

static int  printVariableDefinitions(KBContext *ctx, FILE *file);
static int  printPerceptDefinitions( KBContext *ctx, FILE *file);
static int  printCommonDefinitions( KBContext *ctx, FILE *file);
static int  printPerceptNames( KBContext *ctx, FILE* output);
static int  printVariableNames( KBContext *ctx, FILE* output);

static void printORVariableList(const char* ctx_name, Variable *list, FILE *output);
static void printFileHeader( KBContext *ctx, FILE *output);
static void printInitFunction(KBContext *ctx, FILE *output);
static void printInitFunctionDef(KBContext* ctx, FILE* output);
static void printKBRules(KBContext *ctx, FILE *output);
static void printKBChoiceInfo( IOContext* io_ctx );
static void addAvailableInfo( IOContext* io_ctx, KBContext *kb_ctx );
static void indent(FILE *file, int level);

/**
 * Initialize parser I/O context.
 * Opens output files for writing, and prints out
 * static headers.
 *
 * @param io_ctx - pointer to I/O context storage.
 * @param kb_ctx - pointer to knowledge base context storage.
 * 
 * @return 1 - if initialization is successful.
 *         0 - otherwise.
 */
int
initIOContext(IOContext* io_ctx, KBContext* kb_ctx ) {

    assert( io_ctx != NULL);

    io_ctx->h_file = fopen(INIT_H_FILE, "w");

    if( io_ctx->h_file == NULL ) {
        return 0;
    }

    io_ctx->c_file = fopen(INIT_C_FILE, "w");

    if( io_ctx->c_file == NULL ) {
        return 0;
    }

    printFileHeader          ( kb_ctx, io_ctx->c_file );
    fprintf(io_ctx->c_file, "\n\n#include \"" INIT_H_FILE "\"\n\n" );
    printPerceptNames        ( kb_ctx, io_ctx->c_file );

    printFileHeader          ( kb_ctx, io_ctx->h_file );

    fprintf(io_ctx->h_file, "\n\n#ifndef KOPS_KB_INIT_H\n" );
    fprintf(io_ctx->h_file, "#define KOPS_KB_INIT_H\n" );

    printCommonDefinitions   ( kb_ctx, io_ctx->h_file );
    printPerceptDefinitions  ( kb_ctx, io_ctx->h_file );

    io_ctx->available = NULL;
    io_ctx->available_count = 0;

    return 1;
}

/**
 * Finalize I/O context.
 * Prints out static text to file end and closes output files.
 *
 * @param io_ctx - pointer to I/O context storage.
 */
void
finalizeIOContext( IOContext* io_ctx ) {

    printKBChoiceInfo( io_ctx );

    fprintf(io_ctx->h_file, "#endif /* !KOPS_KB_INIT_H */\n" );

    fclose( io_ctx->c_file );
    fclose( io_ctx->h_file );
}

/**
 * Print out bot specific knowledge base.
 * The printKBFiles -function will bot's KB functions
 * and macro definitions to INIT_C_FILE and INIT_H_FILE
 * respectively.
 *
 * @param io_ctx - pointer to I/O context storage.
 * @param kb_ctx - pointer to knowledge base context storage.
 * 
 * @return 1 - if printing is successful.
 *         0 - otherwise.
 */
int
printKBFiles( IOContext* io_ctx, KBContext *kb_ctx ) {

    assert(io_ctx != NULL);
    assert(kb_ctx != NULL);

    printVariableDefinitions ( kb_ctx, io_ctx->h_file );
    printInitFunctionDef     ( kb_ctx, io_ctx->h_file );

    printVariableNames       ( kb_ctx, io_ctx->c_file );
    printInitFunction        ( kb_ctx, io_ctx->c_file );

    addAvailableInfo( io_ctx, kb_ctx );

    return 1;
}

void
printKBChoiceInfo( IOContext* io_ctx ) {

    KBInfo* iterator;

    assert( io_ctx != NULL );

    fprintf(io_ctx->c_file, "const int kbChoiceCount = %d;\n\n", io_ctx->available_count);
    fprintf(io_ctx->c_file, "KBINFO kbChoice[%d] = {\n", io_ctx->available_count);

    for( iterator = io_ctx->available; iterator != NULL; iterator = iterator->next ) {
	if( iterator != io_ctx->available ) {
	    fprintf(io_ctx->c_file, ",\n");
	}
	indent(io_ctx->c_file, 1);
	fprintf(io_ctx->c_file, "{ \"%s\", " INIT_PREFIX "%s, " VAR_TABLE_PREFIX "%s }\n", 
		iterator->name, iterator->name, iterator->name);
    }

    fprintf(io_ctx->c_file, "\n};\n\n");
}

static 
void addAvailableInfo( IOContext* io_ctx, KBContext *kb_ctx ) {

    KBInfo* info;

    info               = (KBInfo*) safeMalloc( sizeof(KBInfo) );
    info->name         = strdup(kb_ctx->name);
    info->next         = io_ctx->available;
    io_ctx->available  = info;

    ++io_ctx->available_count;
}

static void
indent(FILE *file, int level) {

    int count;

    assert(file != NULL);

    for(count = level * INDENT_WIDTH; count > 0; --count) {
        fprintf(file, " ");
    }
}

static void
printORVariableList(const char* ctx_name, Variable *variable, FILE *output) {

    assert(variable != NULL);
    assert(output != NULL);

    for( ; variable != NULL; variable = variable->next) {
	fprintf(output, "%s_%s", ctx_name, variable->name);
	if(variable->next != NULL) {
	    fprintf(output, " | ");
	}
    }
}

static int
outputDefinitions(FILE* file, char* prefix, int max, Variable table[]) {

    int index;
    int number;
    int longest;
    int length;

    longest = -1;
    for(index = 0; index < max; ++index) {
	length = strlen(table[index].name);
	if( length > longest ) {
	    longest = length;
	}
    }

    number = 1;
    for(index = 0; index < max; ++index) {
	fprintf(file, "#define %s_%s ", prefix, table[index].name);
	for( length = strlen(table[index].name); length < longest; ++length ) {
	    fprintf(file, " ");
	}
	fprintf(file, "%#09x\n", number);
	number <<= 1;
    }

    return 1;
}

static int
printVariableDefinitions(KBContext *ctx, FILE *file) {

#if 0
    int index;
    int number;
#endif

    assert(ctx != NULL);
    assert(file != NULL);

#if 0
    for(index = 0; index < ctx-> variableCount; ++index) {
      fprintf(file, "#define %s_%s %d\n", ctx->name, ctx->variable[index].name, number);
      number <<= 1;
    }
#endif

    fprintf(file, "\n/* Start variable definitions for %s */\n", ctx->name);
    outputDefinitions(file, ctx->name, ctx->variableCount, ctx->variable );
    fprintf(file, "\n/* End variable definitions for %s */\n\n", ctx->name);

    return 1;
}

static int
printPerceptDefinitions(KBContext *ctx, FILE *file) {

#if 0
    int index;
    int number;
    int longest;
    int length;
#endif

    assert(ctx != NULL);
    assert(file != NULL);

    fprintf(file, "\n/* Start percept definitions. */\n");
    outputDefinitions(file, "PERCEPT", ctx->perceptCount, ctx->percept );
    fprintf(file, "\n/* End percept definitions. */\n\n");

#if 0
    longest = -1;
    for(index = 0; index < ctx->perceptCount; ++index) {
	length = strlen(ctx->percept[index].name);
	if( length > longest ) {
	    longest = length;
	}
    }


    number = 1;
    for(index = 0; index < ctx->perceptCount; ++index) {
	fprintf(file, "#define PERCEPT_%s ", ctx->percept[index].name);
	for( length = strlen(ctx->percept[index].name); length < longest; ++length ) {
	    fprintf(file, " ");
	}
	fprintf(file, "%#09x\n", number);
	number <<= 1;
    }
#endif

    return 1;
}

static int
outputNames(FILE* output, char* table_name, char* prefix, int max, Variable table[] ) {

    int index;
    int longest;
    int length;

    fprintf(output, "\n\nconst NAME %s[%d] = {\n", table_name, max);

    longest = -1;
    for(index = 0; index < max; ++index) {
	length = strlen(table[index].name);
	if( length > longest ) {
	    longest = length;
	}
    }

    for(index = 0; index < max; ++index) {
	if( index > 0 ) {
	    fprintf(output, ",\n");
	}

	indent(output, 1);
	fprintf(output, "{ %s_%s, ", prefix, table[index].name);

	for( length = strlen(table[index].name); length < longest; ++length ) {
	    fprintf(output, " ");
	}
	fprintf(output, "\"%s_%s\" }", prefix, table[index].name);
    }

    fprintf(output, "\n};\n");

    return 1;
}

static int
printVariableNames(KBContext* ctx, FILE* output) {

    char buf[ MAX_KBNAME_LEN * 2 ];

    assert(ctx != NULL);
    assert(output != NULL);

    sprintf(buf, "%s%s", VAR_TABLE_PREFIX, ctx->name );

    outputNames(output, buf, ctx->name, ctx->variableCount, ctx->variable);

    return 1;
}

static int
printPerceptNames(KBContext *ctx, FILE *output) {

#if 0
    int index;
    int longest;
    int length;
#endif

    assert(ctx != NULL);
    assert(output != NULL);

    outputNames(output, "percept", "PERCEPT", ctx->perceptCount, ctx->percept);

#if 0
    fprintf(output, "\n\nconst NAME percept[%d] = {\n", ctx->perceptCount);

    longest = -1;
    for(index = 0; index < ctx->perceptCount; ++index) {
	length = strlen(ctx->percept[index].name);
	if( length > longest ) {
	    longest = length;
	}
    }

    for(index = 0; index < ctx->perceptCount; ++index) {
	if( index > 0 ) {
	    fprintf(output, ",\n");
	}

	indent(output, 1);
	fprintf(output, "{ PERCEPT_%s, ", ctx->percept[index].name);

	for( length = strlen(ctx->percept[index].name); length < longest; ++length ) {
	    fprintf(output, " ");
	}
	fprintf(output, "\"PERCEPT_%s\" }", ctx->percept[index].name);
    }

    fprintf(output, "\n};\n");
#endif

    return 1;
}

static void
printFileHeader(KBContext *ctx, FILE *output) {

    assert(ctx    != NULL);
    assert(output != NULL);

    fprintf(output, "/* This file is autogenerated by kb-parser program.\n");
    fprintf(output, " * If you edit this file, it might be overridden and\n"); 
    fprintf(output, " * all changes may be lost. Edit the rule base instead.*/\n\n");

}

static int
printCommonDefinitions(KBContext *ctx, FILE *output) {

    assert(ctx    != NULL);
    assert(output != NULL);

    fprintf(output, "#include \"cannonfodder.h\"\n\n");

    fprintf(output, "/* Start common definitions */\n\n");

    fprintf(output, "#define IGNORE_ALL  0x0\n");
    fprintf(output, "#define MODIFY_NONE 0x0\n\n");
    fprintf(output, "extern const NAME percept[];\n");
    fprintf(output, "/* End common definitions */\n\n");

    return 1;
}

static void 
printInitFunctionDef(KBContext* ctx, FILE* output) {

    assert(ctx    != NULL);
    assert(output != NULL);

    fprintf(output, "extern void " INIT_PREFIX "%s(KNOWLEDGE *kb);\n", ctx->name);
}

static void
printInitFunction(KBContext *ctx, FILE *output) {
    
    int index;

    assert(ctx    != NULL);
    assert(output != NULL);

    fprintf(output, "\n/* Initialize KB.*/\n");
    fprintf(output, "void \n");
    fprintf(output, INIT_PREFIX "%s(KNOWLEDGE *kb) {\n\n", ctx->name);


    indent(output, 1); 
    fprintf(output, "JUMPTARGET* target;\n");
    indent(output, 1); 
    fprintf(output, "RULE* rule;\n\n");
    /*
    indent(output, 1); 
    fprintf(output, "int x,y;\n\n"); */

    indent(output, 1); 
    fprintf(output, "logDebug( DBG_BOT|DBG_GLO|DBG_FUN, \"" INIT_PREFIX "%s(%%#x)\", kb);\n\n", ctx->name);

    indent(output, 1); 
    fprintf(output, "kb->variable_names  = " VAR_TABLE_PREFIX "%s;\n", ctx->name);
    indent(output, 1); 
    fprintf(output, "kb->percept_names  = percept;\n");
    indent(output, 1); 
    fprintf(output, "kb->variable_count  = %d;\n", ctx->variableCount);
    indent(output, 1); 
    fprintf(output, "kb->percept_count  = %d;\n\n", ctx->perceptCount);

    indent(output, 1); 
    fprintf(output, "/* Reserve space for world grid. */\n");
    indent(output, 1); 
    fprintf(output, "kb->square_size  = SHIPW >> 1;\n");
    indent(output, 1); 
    fprintf(output, "kb->grid_width   = (int) (levw / kb->square_size) + 1;\n");
    indent(output, 1); 
    fprintf(output, "kb->grid_height  = (int) (levh / kb->square_size) + 1;\n");
    indent(output, 1); 
    fprintf(output, "kb->grid_size    = sizeof(Uint8) * kb->grid_width * kb->grid_height;\n\n");

    indent(output, 1); 
    fprintf(output, "logDebug( DBG_BOT|DBG_GLO|DBG_DET, \"levw = %%d\", levw);\n");
    indent(output, 1); 
    fprintf(output, "logDebug( DBG_BOT|DBG_GLO|DBG_DET, \"levh = %%d\", levh);\n");
    indent(output, 1); 
    fprintf(output, "logDebug( DBG_BOT|DBG_GLO|DBG_DET, \"kb->square_size = %%d\", kb->square_size);\n");
    indent(output, 1); 
    fprintf(output, "logDebug( DBG_BOT|DBG_GLO|DBG_DET, \"kb->grid_width = %%d\", kb->grid_width);\n");
    indent(output, 1); 
    fprintf(output, "logDebug( DBG_BOT|DBG_GLO|DBG_DET, \"kb->grid_height = %%d\", kb->grid_height);\n");
    indent(output, 1); 
    fprintf(output, "logDebug( DBG_BOT|DBG_GLO|DBG_DET, \"kb->grid_size = %%d\", kb->grid_size);\n\n");

    indent(output, 1); 
    fprintf(output, "kb->grid = (Uint8*) malloc(kb->grid_size);\n\n");

    indent(output, 1); 
    fprintf(output, "memset(kb->grid, (char) SQUARE_UNKNOWN,  kb->grid_size);\n\n");

    indent(output, 1); 
    fprintf(output, "/* Set parameters. */\n");
    for(index = 0; index < ctx->parameterCount; ++index) {
	indent(output, 1); 
	fprintf(output, "kb->%s = %d;\n", ctx->parameter[index].key, ctx->parameter[index].value);
    }

    fprintf(output, "\n");


    indent(output, 1); 
    fprintf(output, "/* Set main weapon preference order. */\n");
    for(index = 0; index < ctx->mainWeaponCount; ++index) {
	indent(output, 1); 
	fprintf(output, "kb->w_main_preference[%s] = %d;\n", 
		ctx->w_main_preference[index].key, ctx->w_main_preference[index].value);
    }

    fprintf(output, "\n");


#if 0
    indent(output, 1); 
    fprintf(output, "kb->player_scan_range     = %d\n", getParameter(ctx, "player_scan_range")->value);
    indent(output, 1); 
    fprintf(output, "kb->bot_scan_range        = %d\n", getParameter(ctx, "bot_scan_range")->value);
    indent(output, 1); 
    fprintf(output, "kb->item_scan_range       = %d\n", getParameter(ctx, "item_scan_range")->value);
    indent(output, 1); 
    fprintf(output, "kb->special_scan_range    = %d\n", getParameter(ctx, "special_scan_range")->value);
    indent(output, 1); 
    fprintf(output, "kb->low_energy_limit      = %d\n", getParameter(ctx, "low_energy_limit")->value);
    indent(output, 1); 
    fprintf(output, "kb->high_energy_limit     = %d\n", getParameter(ctx, "high_energy_limit")->value);
    indent(output, 1); 
    fprintf(output, "kb->w_main_preference = %d\n", getParameter(ctx, "preferred_main_weapon")->value);
#endif

    printKBRules(ctx, output);

    fprintf(output, "}\n\n");
}

static void 
printRule(KBContext* ctx, Rule* rule, FILE* output ) {

    Variable *perceptList;
    Variable *variableList;

    logDebug( DBG_PIO|DBG_GLO|DBG_FUN, "printRule(%#x, %#x, %#x)", ctx, rule, output );    

    if(rule->positive != NULL || rule->negative != NULL) {

	fprintf(output, "\n");

	indent(output, 1); fprintf(output, "rule = makeKBRule();\n");

	perceptList  = makeVariableListOfType(rule->positive, PERCEPT_VARIABLE);
	variableList = makeVariableListOfType(rule->positive, TRUTH_VARIABLE);

	indent(output, 1); fprintf(output, "rule->premise_positive_variables = ");

	if(variableList != NULL) {
	    printORVariableList( ctx->name, variableList, output );
	}
	else {
	    fprintf(output, "IGNORE_ALL");
	}

	fprintf(output, ";\n");
	
	indent(output, 1); fprintf(output, "rule->premise_positive_percepts = ");

	if(perceptList != NULL) {
	    printORVariableList( "PERCEPT", perceptList, output );
	}
	else {
	    fprintf(output, "IGNORE_ALL");
	}

	fprintf(output, ";\n");
	
	deleteVariableList( perceptList );
	deleteVariableList( variableList );
	
	perceptList  = makeVariableListOfType(rule->negative, PERCEPT_VARIABLE);
	variableList = makeVariableListOfType(rule->negative, TRUTH_VARIABLE);
	
	indent(output, 1); fprintf(output, "rule->premise_negative_variables = ");
	
	if(variableList != NULL) {
	    printORVariableList( ctx->name, variableList, output );
	}
	else {
	    fprintf(output, "IGNORE_ALL");
	}
	
	fprintf(output, ";\n");
	
	indent(output, 1); fprintf(output, "rule->premise_negative_percepts = ");
	
	if(perceptList != NULL) {
	    printORVariableList( "PERCEPT", perceptList, output );
	}
	else {
	    fprintf(output, "IGNORE_ALL");
	}
	
	fprintf(output, ";\n");
	
	deleteVariableList( perceptList );
	deleteVariableList( variableList );
	
	/* Print implication rules. */
	variableList = makeVariableListOfType(rule->implies->positive, TRUTH_VARIABLE);
	
	indent(output, 1); fprintf(output, "rule->conclusion_positive_variables = ");
	
	if(variableList != NULL) {
	    printORVariableList( ctx->name, variableList, output );
	}
	else {
	    fprintf(output, "MODIFY_NONE");
	}
	fprintf(output, ";\n");
	
	variableList = makeVariableListOfType(rule->implies->negative, TRUTH_VARIABLE);
	
	indent(output, 1); fprintf(output, "rule->conclusion_negative_variables = ");
	
	if(variableList != NULL) {
	    printORVariableList( ctx->name, variableList, output );
	}
	else {
	    fprintf(output, "MODIFY_NONE");
	}
	fprintf(output, ";\n");
	
	/* Print cmd, special & conclusion. */
	indent(output, 1); 
	fprintf(output, "rule->cmd = %s;\n",
		(rule->implies->cmd != NULL?rule->implies->cmd->opcode:"CMD_NOP"));
	
	indent(output, 1); 
	fprintf(output, "rule->special = %s;\n",
		(rule->implies->special != NULL?rule->implies->special:"NULL"));
	
	indent(output, 1); 
	fprintf(output, "rule->conclusion_action.flags = %s;\n", 
		(rule->implies->action != NULL?rule->implies->action:"ACTION_NOT_DEFINED"));
	
	indent(output, 1);
	fprintf(output, "addKBRule(target, rule);\n");
    }

    logDebug( DBG_PIO|DBG_GLO|DBG_RET, "printRule() return" );    
}

static void
printKBRules(KBContext* ctx, FILE* output) {

    JumpLabel* label;
    Rule*      rule;
    int        id;

    logDebug( DBG_PIO|DBG_GLO|DBG_FUN, "printKBRules(%#x, %#x)", ctx, output );    

    assert(ctx    != NULL);
    assert(output != NULL);


    indent(output, 1); fprintf(output, "/* Rules of %s knowledge base. */\n", ctx->name );

    id = 0;
    for( label = ctx->labels; label != NULL; label = label->next ) {
	/* Give each label unique id. */
	label->id = ++id;

	fprintf(output, "\n");
	indent(output, 1); fprintf(output, "/* Jump target: '%s' */\n", label->name );
	indent(output, 1); fprintf(output, "target = makeJumpTarget(%d);\n", label->id);
	indent(output, 1); fprintf(output, "addJumpTarget(kb, target);\n");

    }

    for( label = ctx->labels; label != NULL; label = label->next ) {

	if( label->rule == NULL ) {
	    logError("Error: empty label.");
	    exit( EXIT_FAILURE );
	}
	fprintf(output, "\n");
	indent(output, 1); fprintf(output, "/* Add rules for jump target '%s' (%d).*/\n", label->name, label->id);
	indent(output, 1); fprintf(output, "target = findJumpTarget(kb, %d);\n", label->id);

	/* Print out rules for the label. */
	for(rule = label->rule; rule != NULL; rule = rule->next) {

	    printRule(ctx, rule, output);

	    if( rule->implies == NULL ) {
		logWarning("Rule without effect, conclusion is NULL.");
		continue;
	    }

	    if( rule->implies->cmd != NULL) {
		if( ! strcmp(rule->implies->cmd->opcode, "CMD_JUMP") ) {
		    if( rule->implies->jumpTo == NULL ) {
			logError("Error: jump command without defined target (jumpTo == NULL.");
			exit( EXIT_FAILURE );
		    }
		    indent(output, 1); fprintf(output, "/* Set jump target. */\n");
		    indent(output, 1); fprintf(output, "setJumpTo(kb, rule, %d);\n", rule->implies->jumpTo->id);
		}
	    }
	}
    }

    label = findLabel( ctx, "start" );
    if( label == NULL ) {
	logError("No start rule defined. Empty rule set.");
	exit( EXIT_FAILURE );
    }

    fprintf(output, "\n");
    indent(output, 1); fprintf(output, "/* Set start rule.*/\n");
    indent(output, 1); fprintf(output, "target = findJumpTarget(kb, %d);\n", label->id);
    indent(output, 1); fprintf(output, "setStartRule(kb, target->rule);\n");

    fprintf(output, "\n");
    indent(output, 1); fprintf(output, "/* All done. */\n");

    logDebug( DBG_PIO|DBG_GLO|DBG_FUN, "printKBRules() return" );    
}

Tokenizer *
createTokenizer(char *filename) {
    
    Tokenizer *tokenizer;

    if(filename == NULL || *filename == '\0') {
        return NULL;
    }

    tokenizer = (Tokenizer *) safeMalloc( sizeof(struct Tokenizer) );

    tokenizer->input = fopen(filename, "r");

    if(tokenizer->input == NULL) {
        free(tokenizer);
        return NULL;
    }

    tokenizer->buffer[0]    = '\0';
    tokenizer->cursor       = 0;
    tokenizer->lineNumber   = 1;
    tokenizer->currentToken = token_start;
    tokenizer->lastToken    = token_start;
    tokenizer->nextToken    = token_start;
    tokenizer->str1[0]      = '\0';
    tokenizer->str2[0]      = '\0';
    tokenizer->currentStr   = tokenizer->str1;
    tokenizer->nextStr      = tokenizer->str2;
    tokenizer->peeked       = 0;

    return tokenizer;
}

void
deleteTokenizer(Tokenizer *tokenizer) {

    if(tokenizer == NULL) {
        return;
    }

    if(tokenizer->input != NULL) {
        fclose(tokenizer->input);
    }

    free(tokenizer);
}

int
nextToken(Tokenizer* tokenizer, int peek) {

    int    token;
    int    tokenLength;
    int    index;
    int    comment;
    char*  result;
    char*  tmp;

    assert(tokenizer != NULL);

    token = tokenizer->lastToken;
    comment     = 0;
    tokenLength = 1;

    /* If next token is not known, check it out. */
    if( ! tokenizer->peeked ) {

	while( 1 ) {

	    /* Check if buffer has more to parse. */
	    if( tokenizer->buffer[tokenizer->cursor] == '\0' ) {
		
		/* Read more text to buffer. */
		result = fgets(tokenizer->buffer, BUFFER_SIZE - 1, tokenizer->input);
		tokenizer->cursor = 0;
		
		 logDebug(DBG_PIO|DBG_STA|DBG_DET, "read %s", tokenizer->buffer); 
		
		/* Check for EOF. */
		if(result == NULL) {
		    logDebug(DBG_PIO|DBG_STA|DBG_DET, "got EOF"); 
		    token = token_eof;
		    tokenLength = 0;
		    tokenizer->buffer[0] = '\0';
		    break; /* break out the while -loop and return EOF. */
		}
	    }
	    
	    /* Skip leading whitespaces. */
	    while( (isspace(tokenizer->buffer[tokenizer->cursor]) || comment) && 
		  tokenizer->buffer[tokenizer->cursor] != '\0') {
		if(tokenizer->buffer[tokenizer->cursor] == '\n') {
		    ++tokenizer->lineNumber;
		    comment = 0; /* End of comment. */
		}
		++tokenizer->cursor;
	    }
	    
	    /* If EOS, read more text. */
	    if( tokenizer->buffer[tokenizer->cursor] == '\0' ) {
		continue; /* need to read more data to buffer. */
	    }
	    
	    /* Check if in the beginning of a comment. */
	    if( tokenizer->buffer[tokenizer->cursor] == '#' ) {
		comment = 1;
		continue; /* Go back to strip comment. */
	    }
	    
	    if(tokenizer->buffer[tokenizer->cursor] == '*') {
		/*++tokenizer->cursor;*/
		token = token_kb_name;
		/*tokenizer->str[0] = '*'; tokenizer->str[1] = '\0';*/
		break;
	    }
	    
	    if(tokenizer->buffer[tokenizer->cursor] == '%') {
		/*++tokenizer->cursor;*/
		token = token_var_def;
		/*tokenizer->str[0] = '%'; tokenizer->str[1] = '\0';*/
		break;
	    }
	    
	    if(tokenizer->buffer[tokenizer->cursor] == ',') {
		/*++tokenizer->cursor;*/
		token = token_and;
		/*tokenizer->str[0] = ','; tokenizer->str[1] = '\0';*/
		break;
	    }
	    
	    if(tokenizer->buffer[tokenizer->cursor] == '^') {
		/*++tokenizer->cursor;*/
		token = token_not;
	    /*tokenizer->str[0] = '^'; tokenizer->str[1] = '\0';*/
		break;
	    }
	    
	    if(tokenizer->buffer[tokenizer->cursor] == '\\') {
		/*++tokenizer->cursor;*/
		token = token_cmd;
		/*tokenizer->str[0] = '\\'; tokenizer->str[1] = '\0';*/
		break;
	    }
	    
	    if(tokenizer->buffer[tokenizer->cursor] == '{') {
		/*++tokenizer->cursor;*/
		token = token_open_rule;
		/*tokenizer->str[0] = '{'; tokenizer->str[1] = '\0';*/
		break;
	    }
	    
	    if(tokenizer->buffer[tokenizer->cursor] == '}') {
		/*++tokenizer->cursor;*/
		token = token_close_rule;
		/*tokenizer->str[0] = '}'; tokenizer->str[1] = '\0';*/
		break;
	    }
	    
	    if(tokenizer->buffer[tokenizer->cursor] == ':' ) {
		/*++tokenizer->cursor;*/
		/*tokenizer->str[0] = ':';*/
		if(tokenizer->buffer[tokenizer->cursor+1] == '=' ) {
		    /*++tokenizer->cursor;*/
		    /*tokenizer->str[1] = '='; tokenizer->str[2] = '\0';*/
		    tokenLength = 2;
		    token = token_assign;
		}
		else {
		    /*tokenizer->str[1] = tokenizer->buffer[tokenizer->cursor]; */
		    /*tokenizer->str[2] = '\0';*/
		    token = token_label;
		}
		break;
	    }
	    
	    if(tokenizer->buffer[tokenizer->cursor] == '=') {
		/*++tokenizer->cursor;*/
		/*tokenizer->str[0] = '=';*/
		if(tokenizer->buffer[tokenizer->cursor+1] == '>' ) {
		    /*tokenizer->str[1] = '>'; tokenizer->str[2] = '\0';*/
		    /*++tokenizer->cursor;*/
		    tokenLength = 2;
		    token = token_implies;
		}
		else {
		    /*tokenizer->str[1] = tokenizer->buffer[tokenizer->cursor]; */
		    /*tokenizer->str[2] = '\0';*/
		    token = token_unknown;
		}
		break;
	    }

	    if(isalpha(tokenizer->buffer[tokenizer->cursor])) {
		token = token_name;
		break;
	    }
	    
	    if(isdigit(tokenizer->buffer[tokenizer->cursor])) {
		token = token_number;
		break;
	    }
	    
	    token = token_unknown;
	    break;
	    
	} /* End while -loop. */
    
	tokenizer->nextToken = token;

	if( token == token_name ) {
	    index       = 0;
	    while( isalnum(tokenizer->buffer[tokenizer->cursor+index])  
		  || tokenizer->buffer[tokenizer->cursor+index] == '_' 
		  || tokenizer->buffer[tokenizer->cursor+index] == '-' 
		  || tokenizer->buffer[tokenizer->cursor+index] == '.' ) {
		tokenizer->nextStr[index] = tokenizer->buffer[tokenizer->cursor+index];
		++index;
	    }
	    tokenizer->nextStr[index] = '\0';
	    tokenLength = index;
	}
	else if ( token == token_number ) {
	    index = 0;
	    while( isdigit(tokenizer->buffer[tokenizer->cursor+index]) ) {
		tokenizer->nextStr[index] = tokenizer->buffer[tokenizer->cursor+index];
		++index;
	    }
	    tokenizer->nextStr[index] = '\0';
	    tokenLength = index;
	    tokenizer->nextNumber = atoi(tokenizer->nextStr);
	}
	else {
	    for(index = 0; index < tokenLength; ++index) {
		tokenizer->nextStr[index] = tokenizer->buffer[tokenizer->cursor+index];
	    }
	    tokenizer->nextStr[index] = '\0';
	}

	/* Update cursor position. */
	tokenizer->cursor += tokenLength;
    }

    /* In this position, the next{Token,Str,Number} contains next token. */

    /* If just peeking next token, return next. */
    if( peek == PEEK_MODE ) {
	/* Setting peeked to 1, causes nextToken() call to skip
	 * token read+parse -loop. If nextToken() call is made with
	 * PEEK_MODE, the call comes right here to return same value again.
	 * If nextToken() call is made with SCAN_MODE, the call
	 * goes right after these if -sentence and returns the cached value.
	 */
	tokenizer->peeked       = 1;
	return tokenizer->nextToken;
    }

    /* Otherwise set current to next and indicate not knowing next. */
    tokenizer->lastToken     = tokenizer->currentToken;
    tokenizer->peeked        = 0;
    tokenizer->currentToken  = tokenizer->nextToken;
    tmp                      = tokenizer->nextStr;
    tokenizer->nextStr       = tokenizer->currentStr;
    tokenizer->currentStr    = tmp;
    tokenizer->currentNumber = tokenizer->nextNumber;

    /*tokenizer->str[index] = '\0';*/

    logDebug(DBG_PIO|DBG_STA|DBG_RET, "nexToken() return token %d '%s'", 
	     tokenizer->currentToken, tokenizer->currentStr);
    
    return tokenizer->currentToken;
}

int
loadKBRules(KBContext *ctx, char *filename) {

    Variable   *variable;
    Rule       *rule;
    Conclusion *conclusion;
    Tokenizer  *tokenizer;
    JumpLabel  *label;

    int         state;
    char        name[BUFFER_SIZE];
    const char* special;
    const char* action;

    enum {
        state_outside_rule,
	state_kb_name,
        state_declaration,
        state_assign_or_label,
        state_assign,
        state_premise,
        state_premise_not,
	state_premise_end,
        state_conclusion,
        state_conclusion_not,
	state_conclusion_end,
	state_conclusion_cmd,
	state_jump,
        state_negation,
        state_end,
        state_error,
	state_max
    };

#if DEBUG
    char *state_name[state_max] = {
        "state_outside_rule",
	"state_kb_name",
        "state_declaration",
        "state_assign_or_label",
        "state_assign",
        "state_premise",
        "state_premise_not",
	"state_premise_end",
        "state_conclusion",
        "state_conclusion_not",
	"state_conclusion_end",
	"state_conclusion_cmd",
	"state_jump",
        "state_negation",
        "state_end",
        "state_error"
    };
#endif

    assert(ctx != NULL);

    if(filename == NULL || *filename == '\0') {
	logError("Illegal filename.");
        return 0;
    }

    tokenizer = createTokenizer( filename );

    if(tokenizer == NULL) {
	logError("Tokenizer creation failed.");
        return 0;
    }

    /* Add first rule's label. 
     * The KB file may optionally have 'start' label or
     * the first rule before any labels is the start.
     */
    label = addLabel(ctx, "start");

    if( label == NULL ) {
	logError("Error; initial label 'start' add failed.");
	exit( EXIT_FAILURE );
    }

    state = state_outside_rule;
    while( state != state_error && nextToken(tokenizer, SCAN_MODE) != token_eof) {

	logDebug(DBG_PIO|DBG_GLO|DBG_DET, "STATE: %s (%d)", state_name[state], state ); 

        switch(state) {
	  case state_outside_rule: /* Initial state */
	    switch(tokenizer->currentToken) {

		/* set KB name. */
	      case token_kb_name:
		state = state_kb_name;
		break;

		/* variable declaration */
	      case token_var_def: 
		state = state_declaration;
		break;
		    
		/* begin rule */
	      case token_open_rule:  
		rule = createRule();
		if(rule == NULL) {
		    logError("Error: new rule creation failed.");
		    exit( EXIT_FAILURE );
		}
		state = state_premise;
		break;
		
		/* parameter assignment or label. */
	      case token_name: 
		strncpy(name, tokenizer->currentStr, BUFFER_SIZE);		
		state = state_assign_or_label;
		break;

		/* end of input file. */
	      case token_eof: 
		state = state_end;
		break;
		
		/* anything else is an error. */
	      default: 
		state = state_error;
		logError("Syntax error: illegal token (%s) outside rule.", tokenizer->currentStr);
		break;
	    } 
	    break; /* end of initial state case */

	  case state_kb_name: /* set KB name. */
	    /* only accepted input is name of the KB. */
	    if(tokenizer->currentToken != token_name) { 
		logError("Syntax error: illegal token (%s) in KB name.", tokenizer->currentStr);
		state = state_error;
		break;
	    }

	    strncpy(ctx->name, tokenizer->currentStr, MAX_KBNAME_LEN);
	    str2upper(ctx->name);
	    state = state_outside_rule;
	    break;

	  case state_declaration:  /* variable declaration */

	    /* only accepted input is name of the variable */
	    if(tokenizer->currentToken != token_name) { 
		logError("Syntax error: illegal token (%s) in declaration.", tokenizer->currentStr);
		state = state_error;
		break;
	    }
	    if( addVariable(ctx, tokenizer->currentStr) == NULL ) {
		logError("Syntax error: illegal token (%s) in declaration.", tokenizer->currentStr);
		state = state_error;
		break;
	    }
	    
	    state = state_outside_rule;
	    break;  /* end of variable declaration case */
	    
	  case state_assign_or_label: /* parameter assignment or label. */

	    switch(tokenizer->currentToken) {

		/* label. */
	      case token_label:
		label = findLabel( ctx, name );
		if( label != NULL ) {
		    if( label->rule != NULL ) {
			logError("Duplicate label '%s'.", name);
			exit( EXIT_FAILURE );
		    }
		}
		else {
		    label = addLabel( ctx, name );
		}

		state = state_outside_rule;
		break;	

	      case token_assign:
		/* Assignment, read value and handle it in next state. */
		state = state_assign;
		break;

	      default:
		logError("Illegal token '%s after name.", tokenizer->currentStr);
		break;
	    }
	    break;

	  case state_assign:

	    /* syntax is 'name of parameter' ':=' 'value as integer' */
	    if(tokenizer->lastToken == token_assign && tokenizer->currentToken == token_number ) {
		if(! setParameter(ctx, name, tokenizer->currentNumber)) {
		    logError("Syntax error: illegal token (%s) in assignment.", tokenizer->currentStr);
		    state = state_error;
		    break;
		}
	    }
	    else {
		logError("Syntax error: illegal token (%s) in assignment.", tokenizer->currentStr);
		state = state_error;
		break;
	    }
	    
	    state = state_outside_rule;
	    break;

	  case state_premise: /* inside rule definition, premise part. */

	    switch(tokenizer->currentToken) {
	      case token_not:
		state = state_premise_not;
		break;
	      case token_name:
		if( (variable = findVariable(ctx, tokenizer->currentStr)) == NULL) {
		    if( (variable = findPercept(ctx, tokenizer->currentStr)) == NULL) {
			logError("Syntax error: unknown variable or percept (%s) in premise.", 
				tokenizer->currentStr);
			state = state_error; /* variable nor percept found */
			break;
		    }
		}
		addPositivePremise(rule, variable);
		state = state_premise_end;
		break;
	      default:
		logError("Syntax error: illegal token (%s) in premise.", tokenizer->currentStr);
		    state = state_error;
		break;
	    }
	    break;

	  case state_premise_not:
	    
	    if(tokenizer->currentToken != token_name) {
		logError("Syntax error: illegal token (%s) in premise after NOT.", tokenizer->currentStr);
		state = state_error;
		break;
	    }
	    if( (variable = findVariable(ctx, tokenizer->currentStr)) == NULL) {
		if( (variable = findPercept(ctx, tokenizer->currentStr)) == NULL) {
		    logError("Syntax error: unknown variable or percept (%s) in negative premise.", 
			    tokenizer->currentStr);
		    state = state_error; /* variable nor percept found */
		    break;
		}
	    }
	    addNegativePremise(rule, variable);
	    state = state_premise_end;
	    break;

	  case state_premise_end: 
	    if ( tokenizer->currentToken == token_and ) {
		state = state_premise;
		break;
	    }
	    if ( tokenizer->currentToken == token_implies ) {
		state = state_conclusion;
		conclusion = createConclusion();
		break;
	    }
	    logError("Syntax error: illegal token (%s) after premise.", tokenizer->currentStr);
	    state = state_error; /* syntax error */
	    break;
	  case state_conclusion: 

	    if ( tokenizer->currentToken == token_cmd ) {
		state = state_conclusion_cmd;
		break;
	    }

	    if ( tokenizer->currentToken == token_not ) {
		state = state_conclusion_not;
		break;
	    }
	    if ( tokenizer->currentToken == token_name ) {
		if ( (variable = findVariable(ctx, tokenizer->currentStr)) != NULL) {
		    addPositiveConclusion(conclusion, variable);
		    state = state_conclusion_end;
		    break;
		}
		
		if ( (special = findSpecial(ctx, tokenizer->currentStr)) != NULL ) {
		    setSpecial(conclusion, special);
		    state = state_conclusion_end;
		    break;
		}
		
		if ( (action = findAction(ctx, tokenizer->currentStr)) != NULL ) {
		    setAction(conclusion, action);
		    state = state_conclusion_end;
		    break;
		}
		logError("Syntax error: unknown variable, special or action (%s) in conclusion.", tokenizer->currentStr);
		state = state_error; /* variable, special nor action found */
		break;
	    }
	    
	    logError("Syntax error: illegal token (%s) in conclusion.", tokenizer->currentStr);
	    state = state_error;
	    break;

	  case state_conclusion_cmd:

	    if(tokenizer->currentToken != token_name) {
		logError("Syntax error: illegal token (%s) in conclusion command.", tokenizer->currentStr);
		state = state_error;
		break;
	    }
	    
	    if( ! setCommand(conclusion, tokenizer->currentStr) ) {
		logError("Syntax error: unknown command (%s) in conclusion command.", tokenizer->currentStr);
		state = state_error;
		break;
	    }

	    if( !strcmp(conclusion->cmd->opcode, "CMD_JUMP" ) ) {
		state = state_jump;
	    }
	    else {
		state = state_conclusion_end;
	    }
	    break;

	  case state_jump:

	    if( tokenizer->currentToken != token_name ) {
		logError("Syntax error: illegal token (%s) in jump command. Label name expected.", 
			 tokenizer->currentStr);
		state = state_error;
		break;
	    }

	    conclusion->jumpTo = addLabel( ctx, tokenizer->currentStr );

	    if( conclusion->jumpTo == NULL ) {
		logError("Error: jump label '%s' set failed.", tokenizer->currentStr);
		state = state_error;
	    }
	    else {
		state = state_conclusion_end;
	    }
	    break;

	  case state_conclusion_not:
	    if(tokenizer->currentToken != token_name) {
		logError("Syntax error: illegal token (%s) in negative conclusion.", 
			tokenizer->currentStr);
		state = state_error;
		break;
	    }
	    
	    if( (variable = findVariable(ctx, tokenizer->currentStr)) == NULL) {
		logError("Syntax error: unknown variable (%s) in negative conclusion.", 
			tokenizer->currentStr);
		state = state_error; /* variable nor percept found */
		break;
	    }
	    addNegativeConclusion(conclusion, variable);
	    state = state_conclusion_end;
	    break;
	  case state_conclusion_end:

	    if(tokenizer->currentToken == token_and) {
		state = state_conclusion;
		break;
	    }

	    if(tokenizer->currentToken == token_close_rule) {
		state = state_outside_rule;
		setConclusion(rule, conclusion);
		addRule(label, rule);
		rule = NULL;
		conclusion = NULL;
		break;
	    }
	    logError("Syntax error: illegal token (%s) after conclusion.", tokenizer->currentStr);
	    state = state_error;
	    break;
	  default:
	    logError("Internal error: unknown state.");
	    state = state_error;
	    break;
        }
    }
    
    deleteTokenizer(tokenizer);
    
    if(state != state_end && state != state_outside_rule) {
	return 0;
    }
    
    return 1;
}
