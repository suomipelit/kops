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

#include <stdbool.h>
#include "SDL_mixer.h"

#include "global.h"
#include "wport.h"
#include "init.h"
#include "sound.h"
#include "sound_helpers.h"

static bool s_initialized = false;
static Mix_Music *s_music;
static Sample *s_samples[SNDEFFECTS];

void sound_init()
{
    if (!snd) {
        return;
    }

    if ((Mix_Init(MIX_INIT_MOD) & MIX_INIT_MOD) != MIX_INIT_MOD) {
        printf("Error initializing SDL_mixer: %s\n", Mix_GetError());
        return;
    }
    if (Mix_OpenAudio(s_samprate, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        Mix_Quit();
        printf("Error opening audio: %s\n", Mix_GetError());
        return;
    }

    s_initialized = true;

    s_music = load_music_file(musicfile);

    int effects;
    for (effects = 0; effects < SNDEFFECTS; effects++) {
        s_samples[effects] = load_sample_file(effnames[effects]);
        if (!s_samples[effects]) {
            printf("Sound error: %s\n", Mix_GetError());
        }
    }

    sound_stopplay();
    sound_play(m_startmenu);
    sound_pvol(64);
}

void sound_deinit()
{
    if (!s_initialized) {
        return;
    }

    sound_stopplay();

    if (s_music) {
        Mix_FreeMusic(s_music);
    }

    int effects;
    for (effects = 0; effects < SNDEFFECTS; effects++) {
        Sample *sample = s_samples[effects];
        if (sample) {
            if (sample->audio_buf) {
                free(sample->audio_buf);
            }
            if (sample->mix_chunk) {
                Mix_FreeChunk(sample->mix_chunk);
            }
            free(sample);
        }
    }

    Mix_CloseAudio();
    Mix_Quit();

    s_initialized = false;
}

void sound_effvol(int volume)
{
    if (!snd || !s_initialized) {
        return;
    }

    volume <<= 1;
    if (volume > 128) {
        volume = 128;
    }

    int effects;
    for (effects = 0; effects < SNDEFFECTS; effects++) {
        Sample *sample = s_samples[effects];
        if (sample && sample->mix_chunk) {
            Mix_VolumeChunk(sample->mix_chunk, volume);
        }
    }
}

void sound_musvol(int volume)
{
    if (!snd || !s_initialized) {
        return;
    }

    volume <<= 1;
    if (volume > 128) {
        volume = 128;
    }
    Mix_VolumeMusic(volume);
}

void sound_pvol(int volume)
{
    if (!snd || !s_initialized) {
        return;
    }

    sound_musvol(volume * s_musvol / 64);
    sound_effvol(volume * s_effvol / 64);
}

/* vol=0-64, panpos=0-255 */
void sound_eff(int sampleno, int volume, int panpos, int freq, int chgroup)
{
    if (!snd || !s_initialized) {
        return;
    }

    Sample *sample = s_samples[sampleno];

    if (!sample) {
        return;
    }

    resample_if_needed(sample, freq);

    const int mix_chan = Mix_PlayChannel(-1, sample->mix_chunk, 0);

    if (mix_chan == -1) {
        printf("Mix_PlayChannel: %s\n", Mix_GetError());
    } else {
        Mix_SetPanning(mix_chan, (Uint8)(255 - panpos), (Uint8)panpos);

        // 0..64 -> 0..128 -> 0..127
        const Uint8 max_volume = 127;
        Uint16 mix_vol = volume << 1;
        if (mix_vol > max_volume) {
            mix_vol = max_volume;
        }
        Mix_Volume(mix_chan, (Uint8)mix_vol);
    }
}

void sound_stopplay()
{
    if (!snd || !s_initialized) {
        return;
    }

    Mix_HaltMusic();
}

void sound_play(int order)
{
    if (!snd || !s_initialized) {
        return;
    }

    sound_stopplay();

    if (s_music) {
        Mix_PlayMusic(s_music, -1);
        Mix_SetMusicPosition(order > 0 ? order - 1 : 0);
        // For the order looping to work correctly, self-built
        // SDL_Mixer runtime is needed, see README.md
    }
}
