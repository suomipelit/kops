@rem Compiles the kb-parser with a recent cygwin installation.
@rem
@rem $Id: compile_kb-parser.bat,v 1.1 2003/04/15 18:15:55 tonic Exp $

gcc-2 -I ..\..\..\..\..\LIBS\SDL\SDL\include kb-parser.c parser-io.c util.c -o kb-parser
