# -*- makefile -*-
# configuration for Linux 64-bit x86 with intel icc using SSE2
CC=icc -m64
CPPFLAGS= -D__builtin_exp2=exp2
ARCHFLAGS= -xSSE2 -msse2 -pc64 
GENFLAGS=  -fPIC -fno-builtin -falign-functions=16
OPTFLAGS=  -O3 -ansi-alias -no-prec-div -fast-transcendentals -rcd -ftz
WARNFLAGS= 

LD=$(CC)
LDFLAGS=   -shared -static-intel
LDLIBS=
TESTLIBS=  -lrt -lm
