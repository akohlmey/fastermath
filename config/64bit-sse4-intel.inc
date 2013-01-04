# -*- makefile -*-
# configuration for Linux 64-bit x86 with intel icc using SSE2
CC=icc
CPPFLAGS= 
ARCHFLAGS= -xSSE4.1 -pc64 
GENFLAGS=  -falign-functions=16 -fPIC -fno-builtin
OPTFLAGS=  -O3 -ansi-alias -no-prec-div -fast-transcendentals -rcd -ftz
WARNFLAGS= 

LD=$(CC) -static-intel
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm
