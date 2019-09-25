# $Id: Makefile,v 1.1 2003/04/15 18:15:55 tonic Exp $

CC=gcc
CFLAGS= -Wall -g -O0 -I/usr/include/SDL
LDFLAGS= -g 
INIT_BASE = kb-init
INIT_DEF = -DINIT_C_FILE=$(INIT_BASE).c -DINIT_H_FILE=$(INIT_BASE).h

all: kb-parser libkopsbot.a

libkopsbot.a:  libkopsbot.a(cannonfodder.o $(INIT_BASE).o special-action.o util.o) 
	ranlib libkopsbot.a
libkopsbot.a(cannonfodder.o): cannonfodder.o
	ar crS libkopsbot.a cannonfodder.o
libkopsbot.a($(INIT_BASE).o): $(INIT_BASE).o
	ar crS libkopsbot.a $(INIT_BASE).o
libkopsbot.a(special-action.o): special-action.o
	ar crS libkopsbot.a special-action.o
libkopsbot.a(util.o): util.o
	ar crS libkopsbot.a util.o

kb-parser: kb-parser.o parser-io.o util.o 
	$(CC) $(LDFLAGS) $(INIT_DEF) -o kb-parser kb-parser.o parser-io.o util.o
kb-parser.o: kb-parser.h parser-io.h config.h util.h
parser-io.o: kb-parser.h parser-io.h config.h util.h
cannonfodder.o: cannonfodder.h util.h config.h util.h $(INIT_BASE).h
special-action.o: cannonfodder.h util.h config.h
$(INIT_BASE).o: util.h $(INIT_BASE).h kb-parser
util.o: util.h
$(INIT_BASE).c $(INIT_BASE).h: *.kb kb-parser
	./kb-parser *.kb

clean:
	rm -rf *.o libkopsbot.a kb-parser
