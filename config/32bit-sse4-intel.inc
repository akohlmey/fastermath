# -*- makefile -*-
# configuration for Linux 32-bit x86 with intel icc using SSE2
CC=icc -m32
CPPFLAGS= 
ARCHFLAGS= -xSSE4.1 -pc64 
GENFLAGS=  -falign-functions=16 -fPIC -fno-builtin
OPTFLAGS=  -O3 -ansi-alias -no-prec-div -fast-transcendentals -rcd -ftz
WARNFLAGS= 

LD=$(CC)
LDFLAGS=   -shared -static-intel
LDLIBS=
TESTLIBS=  -lrt -lm
