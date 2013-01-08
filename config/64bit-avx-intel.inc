# -*- makefile -*-
# configuration for Linux 64-bit x86 with intel icc using SSE2
ALIGNMENT=16
CC=icc
CPPFLAGS= 
ARCHFLAGS= -pc64 -xAVX -g
GENFLAGS=  -falign-functions=$(ALIGNMENT) -fPIC -fno-builtin
OPTFLAGS=  -O3 -ansi-alias -no-prec-div -fast-transcendentals -rcd -ftz
WARNFLAGS= 

LD=$(CC) -static-intel
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm
