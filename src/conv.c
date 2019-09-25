/* conv.c
 * Copyright 1996,2001,2003 Jetro Lauha.
 *
 * $Id: conv.c,v 1.1 2003/04/15 18:15:49 tonic Exp $
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "zlib.h"
#include "wport.h"

//#define DEBUG

/*
  Level format:
  offset    size      info
  0         8     header, "KOPSLEV"+ascii 26
  8         4     level type revision
  12        4     level image packed size (xx)
  16        4     level image width
  20        4     level image height
  24        4     parallax image packed size (yy)
  28        4     parallax image width
  32        4     parallax image height
  36        4     transparency table packed size (zz)
  40        xx    level image packed data
  40+xx     yy    parallax image packed data
  40+xx+yy  zz    transparency table packed data
  40+xx+yy+zz  768   0-255 palette data (PCX style)

  Basegfx format:
  offset    size      info
  0         4     gfx image packed size (xx)
  4         4     gfx width
  8         4     gfx height
  12        xx    gfx image packed data
  12+xx     768   0-255 palette data (PCX style)
*/

#define LEVELREV  1

#define LIBFILE   "kops.jfl"
#define SHIPFILE  "kops.shp"
#define GFXFILE   "kops.gfx"
#define GFXPCX    "base.pcx"

char versmaj=0;
char versmin=4;

char sig[8]={'K','O','P','S','L','E','V',26};
unsigned int revision=LEVELREV;
char *copyright="Copyright 1996,2001,2003 Jetro Lauha.";

void calctransp(char *transp)
{
    int a,c,d,r,g,b,dist,mindist;
    char col = 0;

    for (d=0;d<256;d++)
    {
        for (c=0;c<256;c++)
        {
            r=(cdest[(d<<1)+d]+cdest[(c<<1)+c])>>1;
            g=(cdest[(d<<1)+d+1]+cdest[(c<<1)+c+1])>>1;
            b=(cdest[(d<<1)+d+2]+cdest[(c<<1)+c+2])>>1;
            mindist=0x0fffffff;
            for (a=0;a<256;a++)
            {
                dist=(cdest[(a<<1)+a]-r)*(cdest[(a<<1)+a]-r)+
                    (cdest[(a<<1)+a+1]-g)*(cdest[(a<<1)+a+1]-g)+
                    (cdest[(a<<1)+a+2]-b)*(cdest[(a<<1)+a+2]-b);
                if (dist<mindist)
                {
                    mindist=dist;
                    col=a;
                }
            }
            *(transp+(d<<8)+c)=col;
        }
    }
}

void convert(char *picfile,char *backfile,char *outfile)
{
    PIC pic,back;
    char *pkpic,*pkback,*transp,*pktransp;
    unsigned long pkpiclen,pkbacklen,pktransplen;
    int err,a;
    FILE *fp;
#ifdef DEBUG
    printf("%s %s %s\r\n",picfile,backfile,outfile);
#else
    printf("Working.");
#endif
    util_loadpcxpi(picfile,&pic);
    util_loadpcxpi(backfile,&back);
#ifdef DEBUG
    printf("\r\nPic.w:%d Pic.h:%d Back.w:%d Back.h:%d\r\n",
           pic.w,pic.h,back.w,back.h);
#endif
    pkpiclen=(pic.w*pic.h)+(pic.w*pic.h/1000)+16;
    pkpic=(char *)malloc(pkpiclen);
    pkbacklen=(back.w*back.h)+(back.w*back.h/1000)+16;
    pkback=(char *)malloc(pkbacklen);
    transp=(char *)malloc(65536);
    pktransplen=65536+65536/1000+16;
    pktransp=(char *)malloc(pktransplen);
    if (pktransp==NULL) jerror("Levconv.init: Not enough memory",1);
#ifdef DEBUG
    printf("_pkpiclen:%d _pkbacklen:%d pic.pic/size:%d back.pic/size:%d\r\n",
           pkpiclen,pkbacklen,pic.w*pic.h,back.w*back.h);
#else
    printf(".");
#endif
    err=compress(pkpic,&pkpiclen,pic.pic,pic.w*pic.h);
    if (err!=Z_OK)
    {
        printf("\r\nPIC.compress error! Error code %d.\r\n",err);
        exit(err);
    }
    pkpic=(char *)realloc(pkpic,pkpiclen);
    if (pkpic==NULL)
    {
        printf("\r\nPIC.realloc error!\r\n");
        exit(1);
    }
#ifdef DEBUG
    printf("pkpiclen:%d ",pkpiclen);
#else
    printf(".");
#endif
    err=compress(pkback,&pkbacklen,back.pic,back.w*back.h);
    if (err!=Z_OK)
    {
        printf("\r\nBACK.compress error! Error code %d.\r\n",err);
        exit(err);
    }
    pkback=(char *)realloc(pkback,pkbacklen);
    if (pkback==NULL)
    {
        printf("\r\nBACK.realloc error!\r\n");
        exit(1);
    }
#ifdef DEBUG
    printf("pkbacklen:%d\r\n",pkbacklen);
#else
    printf(".");
#endif

    for (a=0;a<768;a++) cdest[a]=*(pic.pal+a);
    jloadpcxpalpartdest(SHIPFILE,192,255);
    cdest[255*3]=63; cdest[255*3+1]=63; cdest[255*3+2]=63;
    jloadpcxpalpartdest("kops.gfx",128,191);
    for (a=0;a<768;a++) *(pic.pal+a)=cdest[a]<<2;

    calctransp(transp);
#ifdef DEBUG
    printf("transp calced. ");
#else
    printf(".");
#endif
    err=compress(pktransp,&pktransplen,transp,65536);
    if (err!=Z_OK)
    {
        printf("\r\nTRANSP.compress error! Error code %d.\r\n",err);
        exit(err);
    }
    pktransp=(char *)realloc(pktransp,pktransplen);
    if (pktransp==NULL)
    {
        printf("\r\nTRANSP.realloc error!\r\n");
        exit(1);
    }
#ifdef DEBUG
    printf("pktransplen:%d\r\n",pktransplen);
#else
    printf(".");
#endif

    fp=fopen(outfile,"wb");

    fwrite(&sig,8,1,fp);
    fwrite(&revision,4,1,fp);
    fwrite(&pkpiclen,4,1,fp);
    fwrite(&pic.w,4,1,fp);
    fwrite(&pic.h,4,1,fp);
    fwrite(&pkbacklen,4,1,fp);
    fwrite(&back.w,4,1,fp);
    fwrite(&back.h,4,1,fp);
    fwrite(&pktransplen,4,1,fp);
    fwrite(pkpic,pkpiclen,1,fp);
    fwrite(pkback,pkbacklen,1,fp);
    fwrite(pktransp,pktransplen,1,fp);
    fwrite(pic.pal,768,1,fp);

    fclose(fp);

    util_freepi(&pic);
    util_freepi(&back);
    free(pkpic);    pkpic=NULL;
    free(pkback);   pkback=NULL;
    free(transp);   transp=NULL;
    free(pktransp); pktransp=NULL;

#ifdef DEBUG
    printf("    -end-\r\n");
#else
    printf("done!\r\n\r\n");
#endif
}

void makebasegfx(char *gfxfile)
{
    FILE *fp;
    PIC pic;
    unsigned long pkgfxlen;
    char *pkgfx;
    int err,a;

    printf("(Base) Working...");
    util_loadpcxpi(GFXPCX,&pic);
    pkgfxlen=(pic.w*pic.h)+(pic.w*pic.h/1000)+16;
    pkgfx=(char *)malloc(pkgfxlen);
    if (pkgfx==NULL) jerror("Levconv.baseinit: Not enough memory",1);
    err=compress(pkgfx,&pkgfxlen,pic.pic,pic.w*pic.h);
    if (err!=Z_OK)
    {
        printf("\r\nGFX.compress error! Error code %d.\r\n",err);
        exit(err);
    }
    pkgfx=(char *)realloc(pkgfx,pkgfxlen);
    if (pkgfx==NULL)
    {
        printf("\r\nGFX.realloc error!\r\n");
        exit(1);
    }
    for (a=0;a<768;a++) *(pic.pal+a)=*(pic.pal+a)<<2;
    fp=fopen(gfxfile,"wb");
    fwrite(&pkgfxlen,4,1,fp);
    fwrite(&pic.w,4,1,fp);
    fwrite(&pic.h,4,1,fp);
    fwrite(pkgfx,pkgfxlen,1,fp);
    fwrite(pic.pal,768,1,fp);
    fclose(fp);
    util_freepi(&pic);
    printf("done!\r\n\r\n");
}

int main(int argc,char *argv[])
{
    int res;
    printf("\r\nKops CONV version %d.%d - %s\r\n\r\n",versmaj,versmin,copyright);

#ifdef __MSDOS__
    /* exe validation */
    if (argc<1)
    {
        printf("Program file name is unknown.\n\n");
        exit(1);
    }
    switch(jvalidatefile(argv[0]))
    {
        case file_invalid:
            printf("\n\nCRITICAL ERROR:\n\n");
            printf("  This executable file has been modified or corrupted. This\n");
            printf("  can be result of transfer error, hard disk failure, virus\n");
            printf("  or illegal try to modify the data contained by the program. \n");
            printf("  You should obtain a new copy of this program from\n");
            printf("  reputable source.\n\n");
            exit(2);
        case file_failure:
            printf("\n\nCRITICAL ERROR:\n\n");
            printf("  Not enough memory or could not open the program executable.\n");
            exit(3);
    }
#endif
    if ((argc==2)&&(stricmp(argv[1],"base")==0))
    {
        util_fopenlib(LIBFILE);
        makebasegfx(GFXFILE);
        util_fcloselib(LIBFILE);
        exit(0);
    }
    else if (argc<4)
    {
        printf("  Usage: CONV levelfile backgroundfile destination\r\n");
        printf("Example: CONV LEVEL.PCX BACKGR.PCX MYLEVEL.KOP\r\n\r\n");
        printf("Palette is loaded from levelfile. Levelfile and background-\r\n"\
               "file must be standard 256-color PCX files.\r\n\r\n");
        exit(1);
    }
    res=util_fopenlib(LIBFILE);
    if (res < 0)
    {
        printf("Cannot open library file %s (%d)\n", LIBFILE, res);
        exit(1);
    }
    convert(argv[1],argv[2],argv[3]);
    util_fcloselib(LIBFILE);
    return 0;
}
