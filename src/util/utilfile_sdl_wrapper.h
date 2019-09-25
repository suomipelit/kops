/* utilfile_sdl_wrapper.h
 * Provides access to utilfiles via SDL_RWops structure.
 *
 * $Id: utilfile_sdl_wrapper.h,v 1.1 2003/04/15 18:15:57 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef UTIL_SDL_WRAPPER_H_INCLUDED
#define UTIL_SDL_WRAPPER_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#ifdef UTILFILE_H_INCLUDED
#ifdef _SDL_RWops_h


static int util_fseek_SDL_RW(SDL_RWops *context, int offset, int whence)
{
    return util_fseek(
        (UTIL_FILE *)context->hidden.unknown.data1, offset, whence);
}

static int util_fread_SDL_RW(SDL_RWops *context, void *ptr, int size, int maxnum)
{
    return util_fread(
        (char *)ptr, size, maxnum, (UTIL_FILE *)context->hidden.unknown.data1);
}

static int util_write_SDL_RW_err(SDL_RWops *context, const void *ptr, int size, int num)
{
    SDL_SetError("Write not supported for utilfile SDL wrapper!");
    return -1;
}

static int util_fclose_SDL_RW(SDL_RWops *context)
{
    int ret = util_fclose((UTIL_FILE *)context->hidden.unknown.data1);
    if (context)
        free(context);
    return ret;
}

static SDL_RWops *util_fopen_SDL_RW(char *filename)
{
    UTIL_FILE *fp;
    SDL_RWops *rwops = NULL;

    fp = util_fopen(filename);
    if (fp == NULL) {
        SDL_SetError("Couldn't open %s", filename);
        return NULL;
    }

    rwops = SDL_AllocRW();
    if ( rwops != NULL ) {
        rwops->hidden.unknown.data1 = (void *)fp;
        rwops->seek = util_fseek_SDL_RW;
        rwops->read = util_fread_SDL_RW;
        rwops->write = util_write_SDL_RW_err;
        rwops->close = util_fclose_SDL_RW;
    }
    return(rwops);
}



#else
#error SDL_rwops.h REQUIRED
#endif
#else
#error utilfile.h REQUIRED
#endif

#ifdef __cplusplus
};
#endif
#endif

