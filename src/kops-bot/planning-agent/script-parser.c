/* $Id: script-parser.c,v 1.1 2003/04/15 18:15:54 tonic Exp $
 * $Revision: 1.1 $
 */

struct keyword {
    char *name;
    void (*print)();
};

char *grammar = {
    ""};
