# Old crap quickly cut'n'pasted together to compile KOPS
#	-- at
#
# $Id: Rules.make,v 1.1 2003/04/15 18:15:49 tonic Exp $

# Debug?
DEBUG = n

ifeq ($(DEBUG),y)
# Debug flags for any platform
P_FLAGS	 = -O -g
# Dumps core if -pg?
#P_FLAGS	+= -pg
else
# Platform specific flags
# (for i386, size optimized)
#P_FLAGS	 = -O2 -m386 -malign-functions=0 -malign-loops=0 -malign-jumps=0
# (for i486, speed optimized)
#P_FLAGS	 = -O2 -m486 -fno-strength-reduce -funroll-loops
# (for i586 and above, speed optimized)
#P_FLAGS = -O2 -march=i586 -fno-strength-reduce
P_FLAGS	 = -O3 -DNDEBUG
# -march=k6
# -g
# -fno-strength-reduce
endif

# Optimization flags
#O_FLAGS	+= -fomit-frame-pointer -finline-functions

# Warning flags
W_FLAGS	+= -Wall -Wno-char-subscripts
W_FLAGS += -Wredundant-decls
#W_FLAGS += -Wfloat-equal -Wtraditional -Wshadow -Wpointer-arith
#W_FLAGS += -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings
#W_FLAGS += -Wsign-compare -Waggregate-return -Wstrict-prototypes
#W_FLAGS += -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls
#W_FLAGS += -Wnested-externs -Wunreachable-code -Winline

# Ansi C crap
#W_FLAGS	+= -ansi -pedantic -D_XOPEN_SOURCE

# Code generation flags
C_FLAGS	+= -s

ASMFLAGS = -f elf
ARFLAGS	 = rcs

# Want SDL driver?
WANT_SDL_DRIVER = y

INCPATH	 = . -Ijvc -Ifmod -I/usr/include/SDL -I/home/smo/cvs
LIBDIR	 = -L/usr/X11R6/lib -Lfmod -L/home/smo/cvs/kops-bot
LIBS	 = -lm -lz -ljpeg -lkopsbot
FMODLIB	 = -lfmod

ifeq ($(DEBUG), y)
#LIBS	+= -lefence
endif
ifeq ($(WANT_SDL_DRIVER),y)
LIBS	+= -lSDL -lpthread
endif

AS	 = $(CROSS)as
LD	 = $(CROSS)ld
CC	 = $(CROSS)gcc
CPP	 = $(CROSS)g++
AR	 = $(CROSS)ar
NM	 = $(CROSS)nm
STRIP	 = $(CROSS)strip
RANLIB	 = $(CROSS)ranlib

MAKE	 = make
RM	 = rm -f
ASM	 = nasm

ifeq ($(DEBUG),y)
CFLAGS	 = $(P_FLAGS) $(W_FLAGS) -I$(INCPATH)
else
CFLAGS	 = $(P_FLAGS) $(O_FLAGS) $(W_FLAGS) $(C_FLAGS) -I$(INCPATH)
endif

ifeq ($(DEBUG),y)
LDFLAGS	 = $(P_FLAGS) $(W_FLAGS) $(LIBDIR)
else
LDFLAGS	 = $(P_FLAGS) $(O_FLAGS) $(W_FLAGS) $(C_FLAGS) $(LIBDIR)
endif

%.o:	%.c
	$(CC) $(CFLAGS) -o $@ -c $<
%.o:	%.cc
	$(CPP) $(CFLAGS) -o $@ -c $<
%.o:	%.cpp
	$(CPP) $(CFLAGS) -o $@ -c $<
%.o:	%.asm
	$(ASM) $(ASMFLAGS) -o $*.o $*.asm
