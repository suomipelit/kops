/* utilcore.c
 * Copyright 1996,1997,2001,2003 Jetro Lauha.
 *
 * $Id: utilcore.c,v 1.1 2003/04/15 18:15:57 tonic Exp $
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

#ifdef _WIN32
#pragma warning(disable : 4115)
#include <windows.h>
#include <mmsystem.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"


const int jrandseed1=0,jrandseed2=0;

static long randseed = 1L;

void crapsrand(unsigned int seed)
{
    randseed = (long) seed;
}

int craprand(void)
{
    randseed = randseed * 0x343fd + 0x269ec3;
    return (randseed >> 16) & 0x7fff;
}


unsigned int util_gettick()
{
#ifdef _WIN32
    return timeGetTime();
#else
    struct timeval tym;

    gettimeofday(&tym, NULL);
    return (tym.tv_sec * 100) + (tym.tv_usec * 100 / 1000000);
#endif
}

void util_line(int x1,int y1,int x2,int y2,unsigned char c,int pgwid,char *dest)
{
    int i,dx,dy,n,d,di1,di2,xi1,xi2,yi1,yi2;
    int x,y;
    dx=(((x2-x1)<0) ? -(x2-x1) : (x2-x1));
    dy=(((y2-y1)<0) ? -(y2-y1) : (y2-y1));

    if (dx>=dy) {
        n=dx+1;
        d=(dy<<1)-dx;
        di1=(dy<<1);
        di2=((dy-dx)<<1);
        xi1=1; xi2=1; yi1=0; yi2=1;
    } else {
        n=dy+1;
        d=(dx<<1)-dy;
        di1=(dx<<1);
        di2=((dx-dy)<<1);
        xi1=0; xi2=1; yi1=1; yi2=1;
    }
    if (x1>x2) {
        xi1=-xi1;
        xi2=-xi2;
    }
    if (y1>y2) {
        yi1=-yi1;
        yi2=-yi2;
    }
    x=x1;
    y=y1;
    for (i=1;i<=n;i++) {
        *(char *)(dest+y*pgwid+x)=c;
        if (d<0) {
            d+=di1;
            x+=xi1;
            y+=yi1;
        } else {
            d+=di2;
            x+=xi2;
            y+=yi2;
        }
    }
}


void util_lineclip(int x1,int y1,int x2,int y2,int cx1,int cy1,
                   int cx2,int cy2,unsigned char c,int pgwid,char *dest)
{
    int i,dx,dy,n,d,di1,di2,xi1,xi2,yi1,yi2;
    int x,y;
    dx=(((x2-x1)<0) ? -(x2-x1) : (x2-x1));
    dy=(((y2-y1)<0) ? -(y2-y1) : (y2-y1));

    if (dx>=dy) {
        n=dx+1;
        d=(dy<<1)-dx;
        di1=(dy<<1);
        di2=((dy-dx)<<1);
        xi1=1; xi2=1; yi1=0; yi2=1;
    } else {
        n=dy+1;
        d=(dx<<1)-dy;
        di1=(dx<<1);
        di2=((dx-dy)<<1);
        xi1=0; xi2=1; yi1=1; yi2=1;
    }
    if (x1>x2) {
        xi1=-xi1;
        xi2=-xi2;
    }
    if (y1>y2) {
        yi1=-yi1;
        yi2=-yi2;
    }
    x=x1;
    y=y1;
    for (i=1;i<=n;i++) {
        if ((x>=cx1) && (x<cx2) && (y>=cy1) && (y<cy2))
            *(char *)(dest+y*pgwid+x)=c;
        if (d<0) {
            d+=di1;
            x+=xi1;
            y+=yi1;
        } else {
            d+=di2;
            x+=xi2;
            y+=yi2;
        }
    }
}


void util_loadpcxpalptr(char *pal,char *filename)
{
    UTIL_FILE *f;
    unsigned short int a;
    char *buf;
    if (util_finitialized==0) util_finit();
    f=util_fopen(filename);
    if (f!=NULL) {
        buf=(char *)malloc(768);
        util_fseek(f,-768,SEEK_END);
        util_fread(buf,768,1,f);
        for (a=0;a<768;a++) pal[a]=(char)(*(buf+a)>>2);
        free(buf);
        util_fclose(f);
    }
}


#define UTIL_LOADPCX_GETBYT bl++;\
  if (bl>9999) {\
    bl=0; tmp=10000;\
    if (btsl<10000) tmp=btsl;\
    util_fread(buf,tmp,1,f);\
    if (btsl>9999) btsl-=10000;\
  }\
  getbyt=*(buf+bl)


void util_loadpcx(char *dest,char *filename,unsigned int bytes)
{
    unsigned int offs,bl,tmp;
    int btsl;
    unsigned char b1,b2;
    unsigned int a;
    UTIL_FILE *f;
    unsigned char *buf;
    unsigned char getbyt;

    if (util_finitialized==0) util_finit();
    if (util_fileexists(filename)) {
        buf=(unsigned char *)malloc(10000);
        f=util_fopen(filename);
        offs=0;
        util_fseek(f,128,SEEK_SET);
        btsl=util_fsize(f)-128; bl=9999;
        do {
            UTIL_LOADPCX_GETBYT; b1=getbyt;
            if ((b1>>6)==3) {
                UTIL_LOADPCX_GETBYT; b2=getbyt;
                for (a=0;a<(unsigned int)(b1&63);a++) *(dest+a+offs)=b2;
                offs+=(b1&63);
            } else {
                *(dest+offs)=b1;
                offs++;
            }
        } while (offs<bytes);
        util_fclose(f);
        free(buf);
    }
}


/* this ugly old pcx reader load the header straight with fread
 * (which is not the right way)
 */
#pragma pack(1)

typedef struct {
    char manufacturer,version,encoding,bitsperpixel;
    short int xmin,ymin,xmax,ymax;     /* these are used */
    short int hdpi,vdpi;
    char colormap[48];
    char reserved,nplanes;
    short int bytesperline,paletteinfo;
    short int hscreensize,vscreensize;
    char filler[54];
} PCXHEADER;

#pragma pack()


void util_loadpcxpi(char *filename,PIC *pic)
{
    PCXHEADER header;
    unsigned int offs,bl,tmp,bytes=0;
    int btsl;
    unsigned char a,b1,b2;
    UTIL_FILE *f;
    unsigned char *buf,*dest;
    unsigned char getbyt;

    if (util_finitialized==0) util_finit();
    if (util_fileexists(filename)) {
        buf=(unsigned char *)malloc(10000);
        f=util_fopen(filename);
        offs=0;
        util_fread(&header,sizeof(PCXHEADER),1,f);
        pic->bitspp=8;
        pic->bytespp=1;
        pic->w=header.xmax-header.xmin+1;
        pic->h=header.ymax-header.ymin+1;
        bytes=(pic->w)*(pic->h);
        pic->pic=(unsigned char *)malloc(bytes);
        dest=(unsigned char *)pic->pic;
        btsl=util_fsize(f)-128; bl=9999;
        while (offs<bytes) {
            UTIL_LOADPCX_GETBYT; b1=getbyt;
            if ((b1>>6)==3) {
                UTIL_LOADPCX_GETBYT; b2=getbyt;
                for (a=0;a<(b1&63);a++) *(dest+a+offs)=b2;
                offs+=(b1&63);
            } else {
                *(dest+offs)=b1;
                offs++;
            }
        }
        pic->pal=(unsigned char *)malloc(768);
        util_fseek(f,-768,SEEK_END);
        util_fread(pic->pal,768,1,f);
        for (offs=0;offs<768;offs++) *(pic->pal+offs)>>=2;
        util_fclose(f);
        free(buf);
    }
}


void util_loadpcxpihdr(char *filename,PIC *pic)
{
    PCXHEADER header;
    UTIL_FILE *f;

    if (util_finitialized==0) util_finit();
    f=util_fopen(filename);
    util_fread(&header,sizeof(PCXHEADER),1,f);
    pic->w=header.xmax-header.xmin+1;
    pic->h=header.ymax-header.ymin+1;
    util_fclose(f);
}


void util_freepi(PIC *pic)
{
    free(pic->pic); pic->pic=NULL;
    free(pic->pal); pic->pal=NULL;
}


char * util_conv8to32(char *img,char *pal,unsigned long bytes)
{
    unsigned char *tmp;
    unsigned char b;
    int a;
    tmp=(unsigned char *)realloc(img,bytes*4);
    if (tmp==NULL) {
        return(NULL);
    } else {
        for (a=bytes-1;a>=0;a--) {
            b=*(tmp+a);
            *(tmp+a*4)=(char)(pal[b*3+2]<<2);     /* blue */
            *(tmp+a*4+1)=(char)(pal[b*3+1]<<2);   /* green */
            *(tmp+a*4+2)=(char)(pal[b*3]<<2);     /* red */
            *(tmp+a*4+3)=0;               /* alpha */
        }
        return (char *)tmp;
    }
}


#define UTIL_CIRCLEPIX(a,b) { if ((a)>=cx1 && (a)<=cx2 && (b)>=cy1 && (b)<=cy2) *(dest+(b)*pgwid+(a))=c; }

void util_circle(int xx,int yy,int r,int cx1,int cy1,int cx2,int cy2,
                 unsigned char c,int pgwid,char *dest)
{
    int y,d,x,x2,y2;
    y=r;
    d=-r;
    UTIL_CIRCLEPIX(xx,r+yy);
    UTIL_CIRCLEPIX(r+xx,yy);
    UTIL_CIRCLEPIX(xx-r,yy);
    UTIL_CIRCLEPIX(xx,yy-r);
    x=x2=1;         /* x2=2*x-1 */
    y2=-(y<<1)+2;
    while (x<y) {   /* originally x<=y */
        d+=x2;
        if (d>=0) {
            d+=y2;
            y--;
            y2+=2;
        }
        UTIL_CIRCLEPIX(x+xx,y+yy);
        UTIL_CIRCLEPIX(x+xx,-y+yy);
        UTIL_CIRCLEPIX(-x+xx,y+yy);
        UTIL_CIRCLEPIX(-x+xx,-y+yy);
        UTIL_CIRCLEPIX(y+xx,x+yy);
        UTIL_CIRCLEPIX(y+xx,-x+yy);
        UTIL_CIRCLEPIX(-y+xx,x+yy);
        UTIL_CIRCLEPIX(-y+xx,-x+yy);
        x++;
        x2+=2;
    }
}


void util_xorrand(char *data,int jrandseed,int datalen)
{
    int a,saverand;
    saverand=rand();
    crapsrand(jrandseed);
    for (a=0;a<datalen;a++) {
        *(data+a)^=craprand()&255;
    }
    srand(saverand);
}

unsigned int util_filesize(FILE *fp)
{
    int savepos,sizeoffile;
    savepos=ftell(fp);
    fseek(fp,0,SEEK_END);
    sizeoffile=ftell(fp);
    fseek(fp,savepos,SEEK_SET);
    return(sizeoffile);
}


char util_fileexists(char *filename)
{
    UTIL_FILE *fp;
    if (util_finitialized==0) util_finit();
    fp=util_fopen(filename);
    if (fp!=NULL) {
        util_fclose(fp);
        return(1);
    }
    return(0);
}
