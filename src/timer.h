/* $Id: timer.h,v 1.1 2003/04/15 18:15:49 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

extern int timer_rate;
extern void settimer(int frequency);
extern void freetimer();

#endif /* !TIMER_H_INCLUDED */
