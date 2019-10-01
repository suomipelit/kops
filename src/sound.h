/* $Id: sound.h,v 1.1 2003/04/15 18:15:49 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

extern void sound_init();
extern void sound_deinit();
extern void sound_effvol(int volume);
extern void sound_musvol(int volume);
extern void sound_pvol(int volume);
extern void sound_eff(int sampleno, int volume, int panpos, int freq, int chgroup);
extern void sound_play(int order);
extern void sound_stopplay();

#endif /* !SOUND_H_INCLUDED */
