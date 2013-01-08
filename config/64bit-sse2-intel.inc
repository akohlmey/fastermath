# -*- makefile -*-
# configuration for Linux 64-bit x86 with intel icc using SSE2
ALIGNMENT=16
CC=icc
CPPFLAGS= 
ARCHFLAGS= -xSSE2 -pc64 -g
GENFLAGS=  -fPIC -fno-builtin -falign-functions=$(ALIGNMENT)
OPTFLAGS=  -O3 -ansi-alias -no-prec-div -fast-transcendentals -rcd -ftz
WARNFLAGS= 

LD=$(CC) -static-intel
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm
