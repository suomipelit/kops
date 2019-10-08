/* utilfile.h
 *
 * $Id: utilfile.h,v 1.1 2003/04/15 18:15:57 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef UTIL_FILE_H_INCLUDED
#define UTIL_FILE_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stddef.h>

#define UTIL_MAXLIBS 32
#define UTIL_MAXLIBFILES 1024
#define UTIL_FILENAMELEN 40
#define UTIL_INFOLEN 16

#pragma pack(1)

typedef struct {
    FILE *fp;
    int fsize, fstart, pos;
} UTIL_FILE;

typedef char UTIL_LIBFNAMETYPE[UTIL_FILENAMELEN];
typedef char UTIL_LIBINFOTYPE[UTIL_INFOLEN];

typedef struct LIBTYPE {
    char libname[UTIL_FILENAMELEN];
    int files;                // 0 files=deactive library
    int crypt;                // fnames crypted, 0=no, other=yes=seed
    UTIL_LIBFNAMETYPE *fname; // filenames        |
    int *fsize;               // filesizes             |files*item
    int *fstart;              // start offsets         |
    UTIL_LIBINFOTYPE *info;   // other infos      |
} UTIL_LIBTYPE;

#pragma pack()

extern UTIL_LIBTYPE util_flib[UTIL_MAXLIBS];
extern char util_finitialized;
extern char util_fl_sig[4];

extern void util_finit();
extern int util_fopenlib(char *filename);
extern int util_fcloselib(char *filename);
extern void util_fcloselibno(int lib);

extern UTIL_FILE *util_fopen(char *filename);
extern UTIL_FILE *util_fopent(char *filename);
extern int util_fclose(UTIL_FILE *fp);
extern int util_feof(UTIL_FILE *fp);
extern int util_ferror(UTIL_FILE *fp);

extern void util_fgetpos(UTIL_FILE *fp, int *pos);
extern int util_fsetpos(UTIL_FILE *fp, int *pos);
extern size_t util_fread(void *buf, size_t elsize, size_t nelem, UTIL_FILE *fp);
extern int util_fseek(UTIL_FILE *fp, long int offset, int where);

extern long int util_fsize(UTIL_FILE *fp);

#ifdef __cplusplus
};
#endif
#endif
