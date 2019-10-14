#include "sound_helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "SDL_mixer.h"
#include "util/utilfile.h"

Mix_Music *load_music_file(char *filename)
{
    Mix_Music *music = NULL;
    UTIL_FILE *handle = util_fopen(filename);

    if (handle) {
        const size_t data_size = (size_t)handle->fsize;

        if (data_size) {
            Uint8 *buffer = (Uint8 *)malloc(data_size);

            if (buffer) {
                if (util_fread(buffer, handle->fsize, 1, handle)) {
                    SDL_RWops *rwops = SDL_RWFromMem(buffer, (int)data_size);
                    music = Mix_LoadMUSType_RW(rwops, MUS_MOD, SDL_TRUE);
                    // The rwops is freed above by LoadMUS
                }
                free(buffer);
            }
        }
        util_fclose(handle);
    }

    return music;
}

Sample *load_sample_file(char *filename)
{
    Sample *sample = NULL;
    UTIL_FILE *handle = util_fopen(filename);

    if (handle) {
        const size_t data_size = (size_t)handle->fsize;

        if (data_size) {
            Uint8 *buffer = (Uint8 *)malloc(data_size);

            if (buffer) {
                if (util_fread(buffer, handle->fsize, 1, handle)) {
                    sample = (Sample *)malloc(sizeof(Sample));
                    memset(sample, 0, sizeof(Sample));

                    sample->audio_buf = buffer;
                    sample->audio_len = (int)data_size;
                } else {
                    free(buffer);
                }
            }
        }

        util_fclose(handle);
    }

    return sample;
}

void resample_if_needed(Sample *sample, unsigned rate)
{
    if (sample->mix_chunk && sample->mix_rate == rate) {
        // Resampled audio already cached
        return;
    }

    if (sample->mix_chunk) {
        // Free the previous cached audio
        Mix_FreeChunk(sample->mix_chunk);
        sample->mix_chunk = NULL;
    }

    int dev_freq;
    uint16_t dev_format;
    int dev_channels;
    Mix_QuerySpec(&dev_freq, &dev_format, &dev_channels);

    // Convert from desired sampling rate to device sampling rate. This
    // resamples the original as if it already had the desired rate.
    SDL_AudioCVT cvt;
    SDL_BuildAudioCVT(&cvt,
                      AUDIO_S8, 1, rate,
                      dev_format, dev_channels, dev_freq);

    cvt.len = sample->audio_len;
    cvt.buf = (uint8_t *)malloc(cvt.len * cvt.len_mult);
    memcpy(cvt.buf, sample->audio_buf, sample->audio_len);

    SDL_ConvertAudio(&cvt);

    sample->mix_chunk = Mix_QuickLoad_RAW(cvt.buf, cvt.len_cvt);
    sample->mix_rate = rate;
}
