/* utilfile_wrapper.h
 * Hides utilfile calls behind normal fopen/fread/etc...
 *
 * $Id: utilfile_wrapper.h,v 1.1 2003/04/15 18:15:57 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef UTILFILE_WRAPPER_H_INCLUDED
#define UTILFILE_WRAPPER_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#ifdef UTILFILE_H_INCLUDED

#define FILE UTIL_FILE
#define fseek utilfile_fseek
#define ftell utilfile_ftell
#define fgetc utilfile_fgetc
#define fread utilfile_fread
#define fclose utilfile_fclose
#define fopen(a,b) utilfile_fopen(a)

#else
#error utilfile.h REQUIRED
#endif

#ifdef __cplusplus
};
#endif
#endif

