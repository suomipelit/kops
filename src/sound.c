#if 0 // SP-TODO

/* sound.c
 * Copyright 1996,2001,2003 Jetro Lauha.
 *
 * $Id: sound.c,v 1.1 2003/04/15 18:15:49 tonic Exp $
 * $Revision: 1.1 $
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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "wport.h"
#include "init.h"
#include "sound.h"
#include "fmod.h"
#include "fmod_errors.h"

static FMUSIC_MODULE *mod;
static FSOUND_SAMPLE *samp[SNDEFFECTS];
static int curshortchn = 0, curlongchn = 0;
static int muschannels;

static unsigned int fmodopen(const char *name)
{
    return (unsigned int) util_fopen((char *) name);
}

static void fmodclose(unsigned int handle)
{
    util_fclose((UTIL_FILE *) handle);
}

static int fmodread(void *buffer, int size, unsigned int handle)
{
    return util_fread(buffer, 1, size, (UTIL_FILE *) handle);
}

static int fmodseek(unsigned int handle, int pos, signed char mode)
{
    return util_fseek((UTIL_FILE *) handle, pos, mode);
}

static int fmodtell(unsigned int handle)
{
    return util_ftell((UTIL_FILE *) handle);
}

void sound_init()
{
    int eff;

    if (!snd)
	return;
    if (FSOUND_GetVersion() < FMOD_VERSION)
	jerror("FMOD Error: Wrong DLL version! (should be %.02f)", (int) (FMOD_VERSION * 100));

    // kludge: init fsound, load music, get number of channels in music and deinit fsound
    // so we can init fsound again with correct number of channels
    // (=muschannels+totalsfxchannels)
    FSOUND_File_SetCallbacks(fmodopen, fmodclose, fmodread, fmodseek, fmodtell);
    if (!FSOUND_Init(s_samprate, 32, FSOUND_INIT_GLOBALFOCUS))
	jerror(FMOD_ErrorString(FSOUND_GetError()), FSOUND_GetError());
    mod = FMUSIC_LoadSong(musicfile);
    if (!mod)
	jerror(FMOD_ErrorString(FSOUND_GetError()), FSOUND_GetError());
    muschannels = FMUSIC_GetNumChannels(mod);
    FSOUND_Close();

    muschannels += s_shortchn + s_longchn;
    if (muschannels < 32)
	muschannels = 32;

    FSOUND_File_SetCallbacks(fmodopen, fmodclose, fmodread, fmodseek, fmodtell);
    if (!FSOUND_Init(s_samprate, muschannels, FSOUND_INIT_GLOBALFOCUS))
	jerror(FMOD_ErrorString(FSOUND_GetError()), FSOUND_GetError());

    for (eff = 0; eff < SNDEFFECTS; eff++)
    {
	samp[eff] = FSOUND_Sample_Load(FSOUND_FREE,
				       effnames[eff],
				       FSOUND_LOOP_OFF | FSOUND_8BITS |
				       FSOUND_MONO | FSOUND_SIGNED | FSOUND_2D | FSOUND_LOADRAW, 0);
    }
    sound_stopplay();
    sound_play(m_startmenu);
    sound_pvol(64);
}

void sound_deinit()
{
    int a;

    if (!snd)
	return;
    FMUSIC_StopAllSongs();
    for (a = 0; a < SNDEFFECTS; ++a)
	FSOUND_Sample_Free(samp[a]);
    FMUSIC_FreeSong(mod);
    FSOUND_Close();
}

void sound_effvol(int volume)
{
    if (!snd)
	return;
    volume <<= 2;
    if (volume > 255)
	volume = 255;
    FSOUND_SetSFXMasterVolume(volume);
}

void sound_musvol(int volume)
{
    if (!snd)
	return;
    volume <<= 2;
    FMUSIC_SetMasterVolume(mod, volume);
}

void sound_pvol(int volume)
{
    if (!snd)
	return;
    sound_musvol(volume * s_musvol / 64);
    sound_effvol(volume * s_effvol / 64);
}

/* vol=0-64, panpos=0-255 */
void sound_eff(int sampleno, int volume, int panpos, int freq, int chgroup)
{
    int chn, pri;

    if (!snd)
	return;
    if (chgroup)
    {
	chn = muschannels + s_shortchn + curlongchn++;
	pri = 255;
    }
    else {
	chn = muschannels + curshortchn++;
	pri = 50;
    }
    curshortchn %= s_shortchn;
    curlongchn %= s_longchn;
    volume <<= 2;
    if (volume > 255)
	volume = 255;
    chn = FSOUND_PlaySound(FSOUND_FREE, samp[sampleno]);
    FSOUND_SetFrequency(chn, freq);
    FSOUND_SetVolume(chn, volume);
    FSOUND_SetPan(chn, panpos);
    FSOUND_SetPriority(chn, pri);
}

static int playing = 0;

void sound_stopplay()
{
    if (!snd)
	return;
    if (playing)
	FMUSIC_StopAllSongs();
    playing = 0;
}

void sound_play(int order)
{
    if (!snd)
	return;
    sound_stopplay();
    FMUSIC_PlaySong(mod);
    playing = 1;
    FMUSIC_SetOrder(mod, order);
}

#else
void sound_init()
{
}

void sound_deinit()
{
}

void sound_effvol(int volume)
{
}

void sound_musvol(int volume)
{
}

void sound_pvol(int volume)
{
}

void sound_eff(int sampleno, int volume, int panpos, int freq, int chgroup)
{
}

void sound_stopplay()
{
}

void sound_play(int order)
{
}
#endif
