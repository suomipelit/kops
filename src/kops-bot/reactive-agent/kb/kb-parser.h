/* $Id: kb-parser.h,v 1.1 2003/04/15 18:15:56 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef KB_PARSER_H
#define KB_PARSER_H 1

#include "global.h"

/**
 * Limits 
 */
#define MAX_KBNAME_LEN                  32
#define MAX_PERCEPTS                    (sizeof(Uint32) * 8)
#define MAX_TRUTH_VARIABLES             (sizeof(Uint32) * 8)
#define MAX_PARAMETERS                  7

/**
 * Variable types. 
 * Percepts are defined by bot handler and cannot be changed by bot. 
 * Truth variables, on the other hand, are defined in KB file.
 */
#define PERCEPT_VARIABLE    0
#define TRUTH_VARIABLE      1

typedef struct Command {
    const char* token;
    const char* opcode;
} Command;

/**
 * Rule label tracking.
 * For implementing jumping from rule set to an other. 
 */
typedef struct JumpLabel {
    int               id;
    char*             name;
    struct Rule*      rule;
    struct Rule*      last;
    struct JumpLabel* next;
} JumpLabel;

/**
 * Simple key/value parameter implementation type.
 */
typedef struct Parameter {
    char*   key;
    int     value;
} Parameter;


/** 
 * Truth variable structure.
 * Can be used to implement list of known variables.
 */
typedef struct Variable {

    char             type; /* 0 = percept variable, 1 = truth variable */
    const char*      name; /* Name is used to define macro for bit operations. */

    struct Variable* next; /* For list implementation. */

} Variable;

/**
 * Conclusion structure.
 */
typedef struct Conclusion {

    Variable*       positive; /* List of truth variables which will be set true,  if premise is true. */
    Variable*       negative; /* List of truth variables which will be set false, if premise is true. */

    const Command*  cmd;

    JumpLabel*      jumpTo;

    const char*     special;        /* Name of special handler name, i.e. name of the function to call, 
				     * if the premise is true. */

    const char*     action;         /* Name of action name to take, if premise is true. */

} Conclusion;

/**
 * Rule structure.
 */
typedef struct Rule {

    Variable*     positive; /* List of truth variables which shall be true  to make premise true. */
    Variable*     negative; /* List of truth variables which shall be false to make premise true. */

    Conclusion*   implies;  /* If the premise is true, conclusion tells what to do. */

    struct Rule*  next;     /* For list implementation. */
    struct Rule*  subrule;
} Rule;


/**
 * Parser context.
 * Collection of data required during parsing.
 */
typedef struct KBContext {

    char         name[MAX_KBNAME_LEN];

    const char** actionName; /* Possible actions are fixed in game rules. */

    const char** specialName; /* Possible specials are implemented in bot handler. */

    Variable  percept[MAX_PERCEPTS];         /* Known percepts. */
    Variable  variable[MAX_TRUTH_VARIABLES]; /* Known truth variables. */

    /* Some special variables required by the percept creator. 
       These can be set from KB file, and thus stored here before
       the parser outputs bot's rule base.
    */
    Parameter  parameter[MAX_PARAMETERS];
    Parameter  w_main_preference[W_MAIN_MAX];

#if 0 /* Removed, new approach uses JumpLabels. */
    Rule*      rules;  /* List of parsed rules. */
    Rule*      last;
#endif

    JumpLabel* labels; /* List of encountered labels. */

    int        perceptCount;      /* Number of known percepts. */
    int        variableCount;     /* Number of known truth variables. */
    int        parameterCount;    /* Number of parameters. */
    int        mainWeaponCount;   /* Number of main weapons. */

} KBContext;

extern KBContext*  createKBContext();
extern void        resetKBContext(KBContext* ctx);
extern int         optimizeRules(KBContext* ctx);
extern Variable*   addVariable(KBContext* ctx, const char* name);
extern Variable*   addPerceptVariable(KBContext* ctx, const char* name);
extern Variable*   findVariable(KBContext* ctx, const char* name);
extern Variable*   findPercept(KBContext* ctx, const char* name);
extern Rule*       createRule();
extern Variable*   copyVariable(Variable* from);
extern void        addPositivePremise(Rule* rule, Variable* premise);
extern void        addNegativePremise(Rule* rule, Variable* premise);
extern void        deleteVariableList(Variable* start);
extern Variable*   makeVariableListOfType(Variable* iterator, int type);

extern void        setConclusion(Rule* rule, Conclusion* conclusion);
extern Conclusion* createConclusion();
extern void        setSpecial(Conclusion* conclusion, const char* special);
extern void        setAction(Conclusion* conclusion, const char* action);
extern int         setCommand(Conclusion* conclusion, const char* cmd_str);

extern const char* findSpecial(KBContext* ctx, const char* name);
extern const char* findAction(KBContext* ctx, const char* name);

extern int         setParameter(KBContext* ctx, char* key, int value);
extern Parameter*  getParameter(KBContext* ctx, char* key);

extern void        addPositiveConclusion(Conclusion* conclusion, Variable* variable);
extern void        addNegativeConclusion(Conclusion* conclusion, Variable* variable);

extern JumpLabel*  addLabel(KBContext* ctx, const char* label);
extern JumpLabel*  findLabel(KBContext* ctx, const char* label);
extern void        addRule(JumpLabel* label, Rule* rule);

#endif
