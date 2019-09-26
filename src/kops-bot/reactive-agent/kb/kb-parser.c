/* kb-parser.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../../global.h"
#include "kb-parser.h"
#include "parser-io.h"
#include "util.h"

Config config;

static const char* perceptNames[] = {
    "BASE_CLOSE",
    "BOT_CLOSE",
    "PLAYER_CLOSE",
    "FIRED_SPECIAL_CLOSE",
    "FIRED_AMMO_CLOSE",
    "BETTER_MAIN_CLOSE",
    "WORSE_MAIN_CLOSE",
    "SPECIAL_CLOSE",
    "HIGH_ENERGY",
    "LOW_ENERGY",
    "IN_BASE",
    "SLOTS_EMPTY",
    "SLOTS_FULL",
    "MAIN_IS_PREFERRED",
    "GROUND_CLOSE",
    "GROUND_HIT",
    "IN_WATER",
    "STOPPED",
    "LOW_SPEED",
    "MEDIUM_SPEED",
    "HIGH_SPEED",
    "FULL_SPEED",
    "COLLISION_YELLOW",
    "COLLISION_RED",
    NULL
};

static const char* actionNames[] = {
    "ACTION_NOT_DEFINED",
    "ACTION_THRUST",
    "ACTION_TURN_LEFT",
    "ACTION_TURN_RIGHT",
    "ACTION_FIRE_MAIN",
    "ACTION_FIRE_SPECIAL",
    "ACTION_SWITCH_SPECIAL",
    NULL
};

static const char *specialNames[] = {
    NULL
};

/* Known commands. 
 * Note, the opcode's are fixed, the values are macro names
 * which are also used as is in parser-io.c (at least CMD_JUMP).
 */
static const Command commands[] = {
    { "nop",   "CMD_NOP"   },
    { "break", "CMD_BREAK" },
    { "jump",  "CMD_JUMP"  },
    { NULL,    "CMD_NOP"   }
};

static void
printUsage() {
    printf("Usage: kb-parser [-d <debug_lvl>] [-h] <kb_input_file> ...\n");
}

KBContext *
createKBContext() {

    KBContext *ctx;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "createKBContext()" );    

    ctx = (KBContext *) safeMalloc( sizeof(struct KBContext) );

    /*ctx->rules  = NULL;*/
    /*ctx->last   = NULL;*/
    ctx->labels = NULL;

    resetKBContext( ctx );

    return ctx;
}

void
resetKBContext(KBContext* ctx) {

    Rule* rule;
    Rule* rule_tmp;
    JumpLabel* label;
    JumpLabel* label_tmp;

    /* Delete labels. */
    label = ctx->labels;
    while( label != NULL ) {

	if( label->name != NULL ) {
	    free( label->name );
	}

	/* Delete rules. */
	rule = label->rule;
	while(  rule != NULL  ) {

	    deleteVariableList( rule->positive );
	    deleteVariableList( rule->negative );
	    deleteVariableList( rule->implies->positive );
	    deleteVariableList( rule->implies->negative );
	    
	    rule_tmp = rule;
	    rule     = rule->next;
	    
	    free(rule_tmp);
	}

	label_tmp = label;
	label     = label->next;

	free( label_tmp );
    }


    /* Set default context name. */
    strcpy(ctx->name, "unknown");

    /* Reset list pointers. */
    /*ctx->rules  = NULL;*/
    /*ctx->last   = NULL;*/
    ctx->labels = NULL;

    /* Reset variable and percept tables. */
    ctx->variableCount = 0;
    ctx->perceptCount  = 0;

    /* Set default action and special names. */
    ctx->actionName  = actionNames;
    ctx->specialName = specialNames;

    /* Parameter names and default values. */
    ctx->parameter[0].key   = "player_scan_range";
    ctx->parameter[0].value = 100; 
    ctx->parameter[1].key  = "bot_scan_range";
    ctx->parameter[1].value = 100; 
    ctx->parameter[2].key  = "item_scan_range";
    ctx->parameter[2].value = 100; 
    ctx->parameter[3].key =  "ammo_scan_range";
    ctx->parameter[3].value = 100; 
    ctx->parameter[4].key  = "special_scan_range";
    ctx->parameter[4].value = 100; 
    ctx->parameter[5].key  = "low_energy_limit";
    ctx->parameter[5].value = 100; 
    ctx->parameter[6].key  = "high_energy_limit";
    ctx->parameter[6].value = 100; 
    ctx->parameterCount = 7;


    ctx->mainWeaponCount = W_MAIN_MAX;

    /* Main weapon names. */
    ctx->w_main_preference[W_TUHNU].key        =  "W_TUHNU";
    ctx->w_main_preference[W_PEAGUN].key       =  "W_PEAGUN";
    ctx->w_main_preference[W_SLING].key        =  "W_SLING";
    ctx->w_main_preference[W_BIGPEAGUN].key    =  "W_BIGPEAGUN";
    ctx->w_main_preference[W_BIGSLING].key     =  "W_BIGSLING";
    ctx->w_main_preference[W_BIGSLING].key     =  "W_BIGSLING";
    ctx->w_main_preference[W_BLASTGUN].key     =  "W_BLASTGUN";
    ctx->w_main_preference[W_MINIBLAST].key    =  "W_MINIBLAST";
    ctx->w_main_preference[W_BLASTER].key      =  "W_BLASTER";
    ctx->w_main_preference[W_BIGBLASTR].key    =  "W_BIGBLASTR";

    /* Default main weapon preference order. */
    ctx->w_main_preference[W_TUHNU].value      = 1;
    ctx->w_main_preference[W_PEAGUN].value     = 2;
    ctx->w_main_preference[W_SLING].value      = 3;
    ctx->w_main_preference[W_BIGPEAGUN].value  = 4;
    ctx->w_main_preference[W_BIGSLING].value   = 5;
    ctx->w_main_preference[W_BLASTGUN].value   = 6;
    ctx->w_main_preference[W_MINIBLAST].value  = 7;
    ctx->w_main_preference[W_BLASTER].value    = 8;
    ctx->w_main_preference[W_BIGBLASTR].value  = 9;

}

int
setParameter(KBContext *ctx, char *key, int value) {

    int index;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "setParameter(%#x, \"%s\", %d)", ctx, key, value );    

    assert(ctx != NULL);
    assert(key != NULL);

    for(index = 0; index < 8; ++index) {
        if(!strcmp(key, ctx->parameter[index].key)) {
            ctx->parameter[index].value = value;
	    return 1; /* found */
        }
    }

    return 0; /* not found */
}

Parameter*
getParameter(KBContext *ctx, char *key) {
    int index;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "getParameter(%#x, \"%s\")", ctx, key );    

    assert(ctx != NULL);
    assert(key != NULL);

    for(index = 0; index < 8; ++index) {
        if(!strcmp(key, ctx->parameter[index].key)) {
	    return &(ctx->parameter[index]);
	}
    }
    
    return NULL;
}

int 
optimizeRules(KBContext *ctx) {
    return 1;
}

/**
 * Add new truth variable to variable table.
 * Returns NULL, if variable exists already 
 *               or too many variables,
 *               or error occurs.
 *         pointer to variable added, if succesful.
 */
Variable *
addVariable(KBContext *ctx, const char *name) {

    char* str_tmp;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "addVariable(%#x, \"%s\")", ctx, name );    

    assert(ctx != NULL);

    if(name == NULL || *name == '\0' ) {
        return NULL;
    }

    if(findVariable(ctx, name) != NULL) {
        return NULL;
    }

    if(findPercept(ctx, name) != NULL) {
        return NULL;
    }

    if( ctx->variableCount >= MAX_TRUTH_VARIABLES ) {
        return NULL;
    }

    ctx->variable[ctx->variableCount].type = TRUTH_VARIABLE;
    str_tmp = strdup(name);
    str2upper( str_tmp );
    ctx->variable[ctx->variableCount].name = str_tmp;
    ctx->variable[ctx->variableCount].next = NULL;

    return &(ctx->variable[ctx->variableCount++]);
}

Variable *
addPerceptVariable(KBContext *ctx, const char *name) {

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "addPerceptVariable(%#x, \"%s\")", ctx, name );    
    
    assert(ctx != NULL);

    if(name == NULL || *name == '\0' ) {
	logDebug( DBG_MAI|DBG_GLO|DBG_RET, "addPerceptVariable() empty name, return NULL");    
	return NULL;
    }
    
    if(findVariable(ctx, name) != NULL) {
	logDebug( DBG_MAI|DBG_GLO|DBG_RET, "addPerceptVariable() variable found, return NULL");    
	return NULL;
    }

    if(findPercept(ctx, name) != NULL) {
	logDebug( DBG_MAI|DBG_GLO|DBG_RET, "addPerceptVariable() percept found, return NULL");    
	return NULL;
    }

    logDebug( DBG_MAI|DBG_GLO|DBG_DET, "ctx->perceptCount = %d", ctx->perceptCount );    
    logDebug( DBG_MAI|DBG_GLO|DBG_DET, "max perceps = %d", MAX_PERCEPTS );    

    if( ctx->perceptCount >= MAX_PERCEPTS ) {
	logDebug( DBG_MAI|DBG_GLO|DBG_RET, "addPerceptVariable() max percepts, return NULL");    
	return NULL;
    }

    ctx->percept[ctx->perceptCount].type = PERCEPT_VARIABLE;
    /*ctx->percept[ctx->perceptCount].name = strdup(name);*/
    ctx->percept[ctx->perceptCount].name = name;
    /*str2upper( ctx->percept[ctx->perceptCount].name );*/
    ctx->percept[ctx->perceptCount].next = NULL;

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "addPerceptVariable() add ok, return %#x", 
	     &(ctx->percept[ctx->perceptCount]) );

    return &(ctx->percept[ctx->perceptCount++]);
}

/**
 * Find variable from the variable table.
 *
 * @return NULL                - if not found.
 *         pointer to variable - if found.
 */
Variable *
findVariable(KBContext *ctx, const char *name) {

    int   index; 

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "findVariable(%#x, \"%s\")", ctx, name );    

    assert(ctx != NULL);

    if( name == NULL || *name == '\0' ) {
	logDebug( DBG_MAI|DBG_GLO|DBG_RET, "findVariable() empty name, return NULL");    
	return NULL;
    }

    for(index = 0; index < ctx->variableCount; ++index) {

	logDebug( DBG_MAI|DBG_GLO|DBG_DET, "compare to \"%s\"", ctx->variable[index].name);    

	if(!STRICMP(name, ctx->variable[index].name) ) {
	    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "findVariable() found, return %#x", &(ctx->variable[index]));    
	    return &(ctx->variable[index]);
	}
    }

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "findVariable() not found, return NULL");
    return NULL;
}

Variable *
findPercept(KBContext *ctx, const char *name) {

    int index; 

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "findPercept(%#x, \"%s\")", ctx, name );    

    assert(ctx != NULL);

    if( name == NULL || *name == '\0' ) {
	logDebug( DBG_MAI|DBG_GLO|DBG_RET, "findPercept() empty name, return NULL");    
	return NULL;
    }

    for(index = 0; index < ctx->perceptCount; ++index) {

	logDebug( DBG_MAI|DBG_GLO|DBG_DET, "compare to \"%s\"", ctx->percept[index].name);    

	if( !STRICMP(name, ctx->percept[index].name) ) {
	    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "findPercept() found, return %#x", &(ctx->percept[index]));    
	    return &(ctx->percept[index]);
	}
    }

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "findPercept() not found, return NULL");
    return NULL;
}

Rule *
createRule() {

    Rule *rule;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "createRule()" );    

    rule = (Rule *) safeMalloc( sizeof(Rule) );

    rule->positive = NULL;
    rule->negative = NULL;
    rule->implies  = NULL;
    rule->next     = NULL;

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "createRule() return %#x", rule );    

    return rule;
}

Variable *
copyVariable(Variable *from) {

    Variable *variable;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "copyVariable(%#x)", from );    

    assert( from != NULL );

    variable = (Variable *) safeMalloc( sizeof(Variable) );

    variable->type = from->type;
    variable->name = strdup(from->name);
    variable->next = NULL;

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "copyVariable() return %#x", variable );    

    return variable;
}

void
addPositivePremise(Rule *rule, Variable *premise) {

    Variable *tmp;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "addPositivePremise(%#x, %#x)", rule, premise );    

    assert( rule    != NULL );
    assert( premise != NULL );

    tmp = copyVariable(premise);

    tmp->next      = rule->positive;
    rule->positive = tmp;

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "addPositivePremise() return" );    
}

void
addNegativePremise(Rule *rule, Variable *premise) {

    Variable *tmp;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "addNegativePremise(%#x, %#x)", rule, premise );    

    assert( rule    != NULL );
    assert( premise != NULL );

    tmp = copyVariable(premise);

    tmp->next      = rule->negative;
    rule->negative = tmp;

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "addNegativePremise() return" );    
}

Conclusion *
createConclusion() {

    Conclusion *conclusion;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "createConclusion()" );    

    conclusion = (Conclusion *) safeMalloc(sizeof(struct Conclusion));

    conclusion->action    =  NULL;
    conclusion->special   =  NULL;
    conclusion->positive  =  NULL;
    conclusion->negative  =  NULL;
    
    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "createConclusion() return %#x", conclusion );    

    return conclusion;
}

void
deleteVariableList(Variable *start) {

    Variable *tmp;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "deleteVariableList(%#x)", start );    

    while(start != NULL) {
	tmp = start->next;
	free(start);
	start = tmp;
    }

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "deleteVariableList() return" );    
}

Variable *
makeVariableListOfType(Variable *iterator, int type) {

    Variable *copy;
    Variable *list;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "makeVariableListOfType(%#x, %d)", iterator, type );    
    
    list = NULL;
    
    for( ; iterator != NULL; iterator = iterator->next) {
        
        if(iterator->type == type) {
            copy = copyVariable(iterator);
            
            if(copy == NULL) {
                logError("Run out of memory.\n");
                exit(1);
            }
            
            copy->next = list;
            list = copy;
        }
    }

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "makeVariableListOfType() return %#x", list );    

    return list;
}

void
setSpecial(Conclusion *conclusion, const char *special) {

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "setSpecial(%#x, \"%s\")", conclusion, special );    

    conclusion->special = special;

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "setSpecial() return" );    
}

void
setAction(Conclusion *conclusion, const char *action) {

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "setAction(%#x, \"%s\")", conclusion, action );    

    conclusion->action = action;

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "setAction() return" );    
}

const char *
findSpecial(KBContext *ctx, const char *name) {

    int index;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "findSpecial(%#x, \"%s\")", ctx, name );    

    assert(ctx != NULL);
    assert(name != NULL);

    for(index = 0; ctx->specialName[index] != NULL; ++index) {
	if( !strcmp(ctx->specialName[index], name)) {
	    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "findSpecial() found, return \"%s\"", ctx->specialName[index] );    
	    return ctx->specialName[index];
	}
    }

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "findSpecial() not found, return NULL" );    

    return NULL;
}

const char *
findAction(KBContext *ctx, const char *name) {

    int index;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "findAction(%#x, \"%s\")", ctx, name );    

    assert(ctx != NULL);
    assert(name != NULL);

    for(index = 0; ctx->actionName[index] != NULL; ++index) {
	if( !STRICMP(ctx->actionName[index], name)) {
	    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "findAction() found, return \"%s\"", ctx->actionName[index] );    
	    return ctx->actionName[index];
	}
    }

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "findAction() not found, return NULL" );    

    return NULL;
}

void
addPositiveConclusion(Conclusion *conclusion, Variable *variable) {

    Variable *tmp;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "addPositiveConclusion(%#x, %#x)", conclusion, variable );    

    assert(conclusion != NULL);
    assert(variable != NULL);

    tmp = copyVariable(variable);

    tmp->next = conclusion->positive;
    conclusion->positive = tmp;

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "addPositiveConclusion() return" );    
}

void
addNegativeConclusion(Conclusion *conclusion, Variable *variable) {

    Variable *tmp;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "addNegativeConclusion(%#x, %#x)", conclusion, variable );    

    assert(conclusion != NULL);
    assert(variable != NULL);

    tmp = copyVariable(variable);

    tmp->next = conclusion->negative;
    conclusion->negative = tmp;

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "addNegativeConclusion() return" );    
}

int
setCommand(Conclusion* conclusion, const char* cmd_str) {
    int index;

    for( index = 0; commands[index].token != NULL; ++index ) {
	if ( ! strcmp(cmd_str, commands[index].token ) ) {
	    /* Matching command found. */
	    conclusion->cmd = &commands[index];
	    return 1;
	}
    }

    /* No matching command found. */
    return 0;
}

void
setConclusion(Rule *rule, Conclusion *conclusion) {

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "setConclusion(%#x, %#x)", rule, conclusion );    

    assert(rule != NULL);
    assert(conclusion != NULL);

    rule->implies = conclusion;

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "setConclusion() return" );    
}

/**
 * Add rule to end of KB rule list.
 */
void
addRule(JumpLabel* label, Rule *rule) {

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "addRule(%#x, %#x)", label, rule );    

    assert(label != NULL);
    assert(rule  != NULL);

    if(label->rule == NULL) {
	label->rule = rule;
    }
    else {
	label->last->next = rule;
    }

    label->last = rule;

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "addRule() return" );    
    
}

static void
initPercepts(KBContext* ctx) {

    int index;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "initPercepts(%#x)", ctx );    

    for(index = 0; perceptNames[index] != NULL; ++index) {
	addPerceptVariable(ctx, perceptNames[index]);
    }

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "initPercepts() return" );    
}

JumpLabel*
findLabel(KBContext* ctx, const char* str ) {

    JumpLabel* label;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "findLabel(%#x, %s)", ctx, str );    

    assert( ctx != NULL);
    assert( str != NULL);

    for( label = ctx->labels; label != NULL; label = label->next ) {
	if( ! strcmp( label->name, str )) {
	    /* Found. */
	    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "findLabel() found, return %#x", label );    
	    return label;
	}
    }

    /* Not found. */
    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "findLabel() not found, return NULL" );    
    return NULL;
}

JumpLabel*
addLabel(KBContext* ctx, const char* str) {

    JumpLabel* label;

    logDebug( DBG_MAI|DBG_GLO|DBG_FUN, "addLabel(%#x, %s)", ctx, str );    

    label = findLabel( ctx, str );

    if( label != NULL ) {
	logDebug( DBG_MAI|DBG_GLO|DBG_RET, "addLabel() found old, return %#x", label );    
	return label;
    }

    label       = (JumpLabel*) safeMalloc( sizeof(JumpLabel) );
    label->name = strdup(str);
    label->rule = NULL;
    label->next = ctx->labels;
  
    ctx->labels = label;

    logDebug( DBG_MAI|DBG_GLO|DBG_RET, "addLabel() added new, return %#x", label );    

    return label;
}

#if 0 // SP-TODO
int
main(int argc, char *argv[]) {

    KBContext* kb_ctx;
    IOContext  io_ctx;

    int        argno;           /* Used in cmd line option parsing. */
    char       opt_char;        /* Used in cmd line option parsing. */
    char*      optarg;          /* Used in cmd line option parsing. */

    config.debug_lvl   = 0;
    config.debug_log   = stdout;
    config.access_log  = stdout;
    config.error_log   = stderr;

    /* Parse command line options. */
    argno = 0;
    while(++argno < argc && argv[argno][0] == '-' ) {

        opt_char = argv[argno][1];
	optarg   = argv[argno+1];

	switch(opt_char) {
	  case 'd':
	    config.debug_lvl = atoi(optarg);
	    ++argno;
	    break;
	  case 'h':
	    printUsage();
	    exit(EXIT_SUCCESS);      
	  default:
	    logError("Error: unknown option '%c'.\n", opt_char);
	    printUsage();
	    exit(EXIT_FAILURE);
	}
    }

    /* The command line should have at least one  argument, i.e. input filename. */
    if ( argno >= argc) {
	logError("Error: too few arguments, input file missing.\n");
	printUsage();
	exit(EXIT_FAILURE);
    }

    kb_ctx = createKBContext();
	
    initPercepts( kb_ctx );

    if( !initIOContext( &io_ctx, kb_ctx ) ) {
	logError("Error: parser output files open failed.\n" );
	exit(EXIT_FAILURE);
    }

    while( argno < argc ) {
	
	if(! loadKBRules(kb_ctx, argv[argno])) {
	    logError("Error: parsing of file '%s' failed.\n", argv[argno] );
	    exit(EXIT_FAILURE);
	}

	printKBFiles( &io_ctx, kb_ctx ); /*, argv[argno+1]); */
	
	/*printTables( kb_ctx ); */

	resetKBContext( kb_ctx );

	++argno;
    }

    finalizeIOContext( &io_ctx );
    free( kb_ctx );

    return 0;
}
#endif
