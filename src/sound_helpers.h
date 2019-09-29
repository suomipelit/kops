#ifndef SOUND_HELPERS_H_INCLUDED
#define SOUND_HELPERS_H_INCLUDED

#include "SDL_mixer.h"
#include "SDL_stdinc.h"

typedef struct {
    // Original audio
    Uint8 *audio_buf;
    Uint32 audio_len;

    // Last resampled audio
    Mix_Chunk *mix_chunk;
    unsigned int mix_rate;
} Sample;

Mix_Music *load_music_file(char *filename);
Sample *load_sample_file(char *filename);
void resample_if_needed(Sample *sample, unsigned rate);

#endif /* !SOUND_HELPERS_H_INCLUDED */
