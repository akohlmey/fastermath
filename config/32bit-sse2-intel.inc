# -*- makefile -*-
# configuration for Linux 32-bit x86 with intel icc using SSE2
CC=icc -m32
CPPFLAGS= 
ARCHFLAGS= -xSSE2 -pc64 
GENFLAGS=  -fPIC -fno-builtin -falign-functions=16
OPTFLAGS=  -O3 -ansi-alias -no-prec-div -fast-transcendentals -rcd -ftz
WARNFLAGS= 

LD=$(CC) -static-intel
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm
