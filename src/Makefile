# $Id: Makefile,v 1.1 2003/04/15 18:15:49 tonic Exp $

include Rules.make

CFLAGS	+= -DPLATFORM_LINUX

PROGS			= kops conv
UTIL_OBJS		= util/utilcore.o util/utilfile.o
KOPS_OBJS		= kops.o draw.o game.o gamefunc.o init.o wport.o \
			  load.o logo.o menu.o sound.o spr.o timer.o weapon.o
KOPS_BOT_LIB            = kops-bot/reactive-agent/libkopsbot.a

all:			$(PROGS)

kops:			$(KOPS_OBJS) $(UTIL_OBJS) $(KOPS_BOT_LIB)
			$(CC) $(LDFLAGS) $(KOPS_OBJS) $(UTIL_OBJS) $(LIBS) $(FMODLIB) -o kops

conv:			conv.o wport.o $(UTIL_OBJS)
			$(CC) $(LDFLAGS) conv.o wport.o $(UTIL_OBJS) $(LIBS) -o conv

clean:
			$(RM) *.o $(UTIL_OBJS)

cleanall:		clean
			$(RM) $(PROGS)
