/* $Id: cannonfodder.h,v 1.1 2003/04/15 18:15:55 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef KOPS_BOT_CANNONFODDER_H
#define KOPS_BOT_CANNONFODDER_H 1

#include "bot-config.h"
#include "bot-util.h"
#include "../../global.h"

#define FULL_SPEED_LIMIT (FIXS * 28 / 10)
#define HIGH_SPEED_LIMIT (FIXS * 20 / 10)
#define MEDIUM_SPEED_LIMIT (FIXS)
#define LOW_SPEED_LIMIT (FIXS * 1 / 10)

/* Collision alarm signaling distances, i.e. when the alarm is given. */
#define COLLISION_ALARM_RED (SHIPW)
#define COLLISION_ALARM_YELLOW (SHIPW << 1)

/* REMOVED. These are autogenerated with kb-parser. Output is in TABLES_FILE. */
#if 0
/* Pre-defined percepts. */
#define BASE_CLOSE 1                 /* true, iff there is a base inside the base scan range.  */
#define BOT_CLOSE 2                  /* true, iff there is one or more other bots flying inside the bot scan range. */
#define PLAYER_CLOSE (2 << 1)        /* true, iff there is one or more players flying inside the player scan range. */
#define FIRED_SPECIAL_CLOSE (2 << 2) /* true, iff there is fired special inside the fired special scan range. */
#define FIRED_AMMO_CLOSE (2 << 3)    /* true, iff there is fired ammo inside the fired ammo scan range. */
#define BETTER_MAIN_CLOSE (2 << 4)
#define WORSE_MAIN_CLOSE (2 << 5)
#define SPECIAL_CLOSE (2 << 6)
#define HIGH_ENERGY (2 << 7)
#define LOW_ENERGY (2 << 8)
#define IN_BASE (2 << 9)
#define SLOTS_EMPTY (2 << 10)
#define SLOTS_FULL (2 << 11)
#define MAIN_IS_PREFERRED (2 << 12)
#define GROUND_CLOSE (2 << 13)
#define GROUND_HIT (2 << 14)
#define IN_WATER (2 << 15)
#define STOPPED (2 << 16)
#define LOW_SPEED (2 << 17)
#define MEDIUM_SPEED (2 << 18)
#define HIGH_SPEED (2 << 19)
#define FULL_SPEED (2 << 20)

#define FLYING_AROUND
#define COLLECTING_ITEM
#define GOING_BASE (2 << 15)
#define FIGHTING (2 << 16)
#define FLYING_AWAY (2 << 17)
#define AVOID_ITEM (2 << 18)
#define FLEEING (2 << 19)
#define WAITING (2 << 20)
#define HUNTING (2 << 21)
#define DIR_IN_5_DEG (2 << 22)
#define DIR_IN_35_DEG (2 << 23)
#define MOVE_TARGET_SET (2 << 29)
#define FIRE_TARGET_SET (2 << 30)
#define RESERVED_BIT (2 << 31)
#endif

/* Grid square states. */
#define SQUARE_UNKNOWN 0
#define SQUARE_GROUND 1
#define SQUARE_WATER 2
#define SQUARE_BASE 4
#define SQUARE_EMPTY 8

#define RADAR_RANGE 6
#define BEAM_MAX 8

typedef Uint32 flag_t;

/**
 * World grid location state context. 
 */
typedef struct SQUARE {

    int x;     /* x-location of square in grid. */
    int y;     /* y-location of square in grid. */
    int state; /* current state of square. */

} SQUARE;

struct AGENT;

typedef struct PERCEPT {

    flag_t flags;             /* percept propositions. */
    PLAYER *player;           /* closest enemy player. */
    struct AGENT *bot;        /* closest enemy bot. */
    SQUARE *base;             /* closest base. */
    AMMO *special;            /* closest fired special weapon ammo. */
    AMMO *ammo;               /* closest fired main weapon ammo. */
    PICKAMMO *better_main;    /* closest better main weapon item. */
    PICKAMMO *worse_main;     /* closest worse main weapon item. */
    PICKAMMO *pick_special;   /* closest special weapon item. */
    SQUARE location;          /* Bot location in grid. */
    SQUARE beam[RADAR_RANGE]; /* Radar beam percept. */
    int radar_phase;          /* Which quarter is currently beamed [1-4]. */
    int beam_phase;           /* Beam index number. */

} PERCEPT;

/**
 * Possible actions:                   Flag:
 * thrust (move forward),                 1
 * turn left,                             2
 * turn right,                            4
 * fire main weapon,                      8
 * fire special weapon.                  16
 * switch special weapon (only in base), 32
 */

#define ACTION_NOT_DEFINED 0
#define ACTION_THRUST 1
#define ACTION_TURN_LEFT 2
#define ACTION_TURN_RIGHT 4
#define ACTION_FIRE_MAIN 8
#define ACTION_FIRE_SPECIAL 16
#define ACTION_SWITCH_SPECIAL 32

#define SET_ACTION(FLAGS, ACTION) (FLAGS |= ACTION)
#define CLR_ACTION(FLAGS, ACTION) (FLAGS &= (255 - ACTION))
#define IS_ACTION_SET(FLAGS, ACTION) ((FLAGS & ACTION) == ACTION)

/**
 * Conclusion commands.
 */
#define CMD_NOP 0
#define CMD_BREAK 1
#define CMD_JUMP 2

typedef struct NAME {
    int key;
    const char *name;
} NAME;

typedef struct ACTION {

    Uint8 flags; /* action flags */
    Uint8 slot;  /* if action is switch special weapon, slot tells to witch weapon. */

    int move_target_x;
    int move_target_y;

    int fire_target_x;
    int fire_target_y;

    int target_speed;

} ACTION;

typedef struct SPECIAL {
    int (*handler)(struct KNOWLEDGE *kb); /* Function pointer to special handler. */
    struct SPECIAL *next;
} SPECIAL;

/**
 * Rule context.
 * The rule is checked with percept to determine next action.
 */
struct KNOWLEDGE;
typedef struct RULE {

    flag_t premise_positive_variables; /* Variables which has to be true to make rule valid. */
    flag_t premise_negative_variables; /* Variables which has to be false to make rule valid. */

    flag_t premise_positive_percepts; /* Percepts which has to be true to make rule valid. */
    flag_t premise_negative_percepts; /* Percepts which has to be false to make rule valid. */

    flag_t conclusion_positive_variables; /* Variables which will be set true when rule is valid. */
    flag_t conclusion_negative_variables; /* Variables which will be set false when rule is valid. */

    SPECIAL *premise_specials;    /* Functions to be checked as premise. */
    SPECIAL *conclusion_specials; /* Functions to be called when premise is true. */
    SPECIAL *premise_specials_last;
    SPECIAL *conclusion_specials_last;

    ACTION conclusion_action; /* Action to be taken when rule is valid. */

    int cmd; /* Conclusion command. */
    struct JUMPTARGET *jump_to;

    struct RULE *next;      /* Rule list's next rule. */
    struct RULE *jump_next; /* Jump target list's next rule. */

    void (*special)(struct KNOWLEDGE *);

} RULE;

typedef struct JUMPTARGET {
    int id;
    RULE *rule;
    RULE *last;
    struct JUMPTARGET *next;
} JUMPTARGET;

/**
 * Rules to determine actions and state of the world.
 */
typedef struct KNOWLEDGE {

    const NAME *variable_names;
    const NAME *percept_names;

    int variable_count;
    int percept_count;

    RULE *start;
    JUMPTARGET *jump_targets;

    PERCEPT *percept;

    int square_size;
    int grid_width;
    int grid_height;
    int grid_size;

    Uint8 *grid;

    flag_t variables;

    int player_scan_range;
    int bot_scan_range;

    int item_scan_range;
    int ammo_scan_range;
    int special_scan_range;

    int low_energy_limit;
    int high_energy_limit;
    int w_main_preference[W_MAIN_MAX];

    ACTION last_action;

} KNOWLEDGE;

/**
 * Agent context.
 */
typedef struct AGENT {

    PLAYER *remote;  /* A ship the agent controls. */
    PERCEPT percept; /* Last percept of the agent. */
    ACTION action;   /* Last action of the agent. */
    KNOWLEDGE base;  /* Agent's knowledge, rules and state of world. */

} AGENT;

typedef struct KBINFO {
    const char *name;
    void (*init)(KNOWLEDGE *kb);
    const NAME *variable_names;
} KBINFO;

extern KBINFO kbChoice[];
extern const int kbChoiceCount;

extern void botHandler();
extern RULE *makeKBRule();
extern JUMPTARGET *makeJumpTarget(int id);
extern void addKBRule(JUMPTARGET *kb, RULE *rule);
extern JUMPTARGET *findJumpTarget(KNOWLEDGE *kb, int id);
extern void addJumpTarget(KNOWLEDGE *kb, JUMPTARGET *target);
extern void setJumpTo(KNOWLEDGE *kb, RULE *rule, int id);
extern void setStartRule(KNOWLEDGE *kb, RULE *rule);
extern void removeKBRules(KNOWLEDGE *kb);
extern void detachAgents();
extern void attachAgent(PLAYER *plr_ptr);
extern void addSpecialPremise(RULE *rule, SPECIAL *special);
extern void addSpecialConclusion(RULE *rule, SPECIAL *special);
/**
 * Interesting propositions:
 * agent's position, (only interesting when compared to others)
 * agent's velocity,
 * agent's moving direction,
 * agent's face direction,
 * agent's current main weapon,
 * agent's current special weapon,
 * agent's energy,
 * enemies' position,
 * enemies' current main weapon,
 * enemies' current special weapon,
 * enemies' energy,
 * special weapons inventory,
 * ammo of special weapons,
 * location of bases,
 * presence of items,
 * presence of mines,
 * presence of missiles,
 * maximum amount of special weapons available,
 * world.
 */

/**
 * Simple rules:
 * ^enemy close & ^missile &  full energy & ^item close                       ->  fly around
 * ^enemy close & ^in base &  ^missile & ^full energy                         ->  go base to repair
 * ^enemy close &  in base &  ^missile & ^full energy                         ->  wait repair
 * ^enemy close &  in base &  ^missile & several special weapons              ->  select best special weapon
 *  enemy close & ^low energy & weapon better than enemys                     ->  fire towards enemy
 *  enemy close & ^low energy & weapon worse than enemys & no special weapons ->  flee
 *  enemy close &  low energy                                                 ->  flee
 *  enemy close & special weapon & special weapon fits to situation           ->  fire special weapon
 *   item close & ^missile & item main weapon & current worse weapon          ->  collect item
 *   item close & ^missile & item main weapon & current better weapon         -> ^collect item
 *   item close & ^missile & item special weapon & slots empty                ->  collect item
 *   item close & ^missile & item special weapon & slots full                 -> ^collect item
 *  ^item close                                                               -> ^collect item
 *   mine close                                                               ->  fly away
 *  ^mine close                                                               -> ^flying away
 */

#endif
