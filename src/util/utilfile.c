/* utilfile.c
 * Copyright 1996,1997,2001,2003 Jetro Lauha.
 *
 * $Id: utilfile.c,v 1.1 2003/04/15 18:15:57 tonic Exp $
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

#include "util.h"

#define UTIL_MAXLIBS     32
#define UTIL_MAXLIBFILES 1024
#define UTIL_FILENAMELEN 40
#define UTIL_INFOLEN     16

/*
  info bytes:
  0 = packed flag, 0=no pack, 1=zlib, 2=lzss
  1 = crypt flag, 0=no crypt, 1=prng, 2=key (utilfile doesn't have seed/key)
  2-15 = reserved

  file format:
  char signature[4];    // 'J','F','L',26
  int files;            // 0 files=deactive library
  int crypt;            // fnames crypted, 0=no, other=yes=seed
  UTIL_LIBFNAMETYPE *fname;  // filenames            \
  int *fsize;           // filesizes             \_files*item
  int *fstart;          // start offsets         /
  LIBINFOTYPE *info;    // other infos          /
*/

UTIL_LIBTYPE util_flib[UTIL_MAXLIBS];
char util_finitialized=0;
char util_fl_sig[4]={'J','F','L','V'};


void util_finit()
{
    int a;
    util_finitialized=1;
    for (a=0;a<UTIL_MAXLIBS;a++) {
        util_flib[a].files=0;
        memset(&util_flib[a].libname,0,UTIL_FILENAMELEN);
    }
}


UTIL_FILE *util_fopen(char *filename)
{
    UTIL_FILE *ufp;
    char mode=0;
    int a,b,lib=0,fno=0;

    ufp=(UTIL_FILE *)malloc(sizeof(UTIL_FILE));
    for (b=0;b<UTIL_MAXLIBS;b++) if (util_flib[b].files>0) for (a=0;a<util_flib[b].files;a++)
        if (stricmp(filename,util_flib[b].fname[a])==0) { lib=b; fno=a; mode=1; }
    if (mode==1) {              /* ufl */
        ufp->fsize=util_flib[lib].fsize[fno];
        ufp->fstart=util_flib[lib].fstart[fno];
        ufp->fp=fopen(util_flib[lib].libname,"rb");  /* binary file */
        fseek(ufp->fp,ufp->fstart,SEEK_SET);
        ufp->pos=ufp->fstart;
    } else {                    /* normal file */
        ufp->fp=fopen(filename,"rb");             /* binary file */
        if (ufp->fp!=NULL) {
            ufp->fsize=util_filesize(ufp->fp);
            ufp->fstart=0;
            ufp->pos=0;
        } else {
            free(ufp);
            return(NULL);
        }
    }
    return(ufp);
}


UTIL_FILE *util_fopent(char *filename)
{
    UTIL_FILE *ufp;
    char mode=0;
    int a,b,lib=0,fno=0;

    ufp=(UTIL_FILE *)malloc(sizeof(UTIL_FILE));
    for (b=0;b<UTIL_MAXLIBS;b++) if (util_flib[b].files>0) for (a=0;a<util_flib[b].files;a++)
        if (stricmp(filename,util_flib[b].fname[a])==0) { lib=b; fno=a; mode=1; }
    if (mode==1) {              /* ufl */
        ufp->fsize=util_flib[lib].fsize[fno];
        ufp->fstart=util_flib[lib].fstart[fno];
        ufp->fp=fopen(util_flib[lib].libname,"rt");  /* text file */
        fseek(ufp->fp,ufp->fstart,SEEK_SET);
        ufp->pos=ufp->fstart;
    } else {                    /* normal file */
        ufp->fp=fopen(filename,"rt");             /* text file */
        if (ufp->fp!=NULL) {
            ufp->fsize=util_filesize(ufp->fp);
            ufp->fstart=0;
            ufp->pos=0;
        } else {
            free(ufp);
            return(NULL);
        }
    }
    return(ufp);
}


size_t util_fread(void *buf,size_t elsize,size_t nelem,UTIL_FILE *fp)
{
    size_t relem=nelem;
    int bytes;
    bytes=elsize*nelem;
    if (bytes>(fp->fstart)+(fp->fsize)-(fp->pos)) {
        relem=((fp->fstart)+(fp->fsize)-(fp->pos))/elsize;
        bytes=relem*elsize;
    }
    if (relem==0) return(0);
    fread(buf,elsize,relem,fp->fp);
    fp->pos+=bytes;
    return(relem);
}


int util_fopenlib(char *filename)
/*  returns
    0..MAXLIBS-1 = success (library number which is used)
    -1 = no free library place
    -2 = can't open library file
    -3 = invalid library signature
    -4 = no files in library
    -5 = not enough memory
*/
{
    int a,lib=UTIL_MAXLIBS;
    char sig[4];
    FILE *fp;

    if (util_finitialized==0) util_finit();
    for (a=0;a<UTIL_MAXLIBS;a++) if (util_flib[a].files==0) lib=a;
    if (lib==UTIL_MAXLIBS) return(-1);
    strncpy(util_flib[lib].libname,filename, 40);
    fp=fopen(filename,"rb");
    if (fp==NULL) return(-2);
    fread(&sig,4,1,fp);
    if (memcmp(&sig,&util_fl_sig,4)!=0) return(-3);
    fread(&util_flib[lib].files,4,1,fp);
    if (util_flib[lib].files==0) return(-4);
    fread(&util_flib[lib].crypt,4,1,fp);
    util_flib[lib].fname=(UTIL_LIBFNAMETYPE *)
        malloc(util_flib[lib].files*sizeof(UTIL_LIBFNAMETYPE));
    if (util_flib[lib].fname==NULL) { util_flib[lib].files=0; return(-5); }
    util_flib[lib].fsize=(int *)malloc(util_flib[lib].files*sizeof(int));
    if (util_flib[lib].fsize==NULL) { util_flib[lib].files=0; return(-5); }
    util_flib[lib].fstart=(int *)malloc(util_flib[lib].files*sizeof(int));
    if (util_flib[lib].fstart==NULL) { util_flib[lib].files=0; return(-5); }
    util_flib[lib].info=(UTIL_LIBINFOTYPE *)malloc(util_flib[lib].files*sizeof(UTIL_LIBINFOTYPE));
    if (util_flib[lib].info==NULL) { util_flib[lib].files=0; return(-5); }
    fread(util_flib[lib].fname,util_flib[lib].files*sizeof(UTIL_LIBFNAMETYPE),1,fp);
    if (util_flib[lib].crypt>0) util_xorrand((char *)util_flib[lib].fname,
                                             util_flib[lib].crypt,util_flib[lib].files*sizeof(UTIL_LIBFNAMETYPE));
    fread(util_flib[lib].fsize,util_flib[lib].files*sizeof(int),1,fp);
    if (util_flib[lib].crypt>0) util_xorrand((char *)util_flib[lib].fsize,
                                             util_flib[lib].crypt,util_flib[lib].files*sizeof(int));
    fread(util_flib[lib].fstart,util_flib[lib].files*sizeof(int),1,fp);
    if (util_flib[lib].crypt>0) util_xorrand((char *)util_flib[lib].fstart,
                                             util_flib[lib].crypt,util_flib[lib].files*sizeof(int));
    fread(util_flib[lib].info,util_flib[lib].files*sizeof(UTIL_LIBINFOTYPE),1,fp);
    if (util_flib[lib].crypt>0) util_xorrand((char *)util_flib[lib].info,
                                             util_flib[lib].crypt,util_flib[lib].files*sizeof(UTIL_LIBINFOTYPE));
    fclose(fp);
    return(lib);
}


int util_fcloselib(char *filename)
/* returns 0 if success */
{
    int a,lib=UTIL_MAXLIBS;
    for (a=0;a<UTIL_MAXLIBS;a++) if (stricmp(filename,util_flib[a].libname)==0) lib=a;
    if (lib==UTIL_MAXLIBS) return(-1);
    util_flib[lib].files=0;
    util_flib[lib].crypt=0;
    free(util_flib[lib].fname);
    free(util_flib[lib].fsize);
    free(util_flib[lib].fstart);
    free(util_flib[lib].info);
    memset(&util_flib[lib].libname,0,UTIL_FILENAMELEN);
    return(0);
}


void util_fcloselibno(int lib)
{
    util_flib[lib].files=0;
    util_flib[lib].crypt=0;
    free(util_flib[lib].fname);
    free(util_flib[lib].fsize);
    free(util_flib[lib].fstart);
    free(util_flib[lib].info);
    memset(&util_flib[lib].libname,0,UTIL_FILENAMELEN);
}


void util_fclosealllibs()
{
    int lib;
    for (lib=0;lib<UTIL_MAXLIBS;lib++) if (util_flib[lib].files>0) {
        util_flib[lib].files=0;
        util_flib[lib].crypt=0;
        free(util_flib[lib].fname);
        free(util_flib[lib].fsize);
        free(util_flib[lib].fstart);
        free(util_flib[lib].info);
        memset(&util_flib[lib].libname,0,UTIL_FILENAMELEN);
    }
}


int util_fclose(UTIL_FILE *fp)
{
    fclose(fp->fp);
    free(fp);
    return(0);
}


int util_feof(UTIL_FILE *fp)
{
    if ((fp->pos>=(fp->fstart+fp->fsize))||(feof(fp->fp)))
        return(-1); else return(0);
}


int util_ferror(UTIL_FILE *fp)
{
    if ((fp->pos>=(fp->fstart+fp->fsize))||(ferror(fp->fp)))
        return(-1); else return(0);
}


int util_fgetc(UTIL_FILE *fp)
{
    if (fp->pos<fp->fstart+fp->fsize) {
        fp->pos++;
        return(fgetc(fp->fp));
    } else return(EOF);
}


int util_fseek(UTIL_FILE *fp,long int offset,int where)
{
    int orgpos=fp->pos;
    switch(where) {
        case SEEK_SET:
            fp->pos=fp->fstart+offset;
            fseek(fp->fp,fp->pos,SEEK_SET);
            break;
        case SEEK_CUR:
            fp->pos+=offset;
            fseek(fp->fp,fp->pos,SEEK_SET);
            break;
        case SEEK_END:
            fp->pos=fp->fstart+fp->fsize+offset;
            fseek(fp->fp,fp->pos,SEEK_SET);
            break;
    }
    if ((fp->pos<fp->fstart)||(fp->pos>fp->fstart+fp->fsize)) {
        fp->pos=orgpos;
        fseek(fp->fp,orgpos,SEEK_SET);
        return(-1);
    } else return(0);
}


long int util_ftell(UTIL_FILE *fp)
{
    return((fp->pos)-(fp->fstart));
}


long int util_fsize(UTIL_FILE *fp)
{
    return(fp->fsize);
}


long int util_fstart(UTIL_FILE *fp)
{
    return(fp->fstart);
}
