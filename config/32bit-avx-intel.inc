# -*- makefile -*-
# configuration for Linux 32-bit x86 with intel icc using AVX
ALIGNMENT=32
CC=icc -m32
CPPFLAGS= 
ARCHFLAGS= -pc64 -xAVX -g
GENFLAGS=  -falign-functions=16 -fPIC -fno-builtin
OPTFLAGS=  -O3 -ansi-alias -no-prec-div -fast-transcendentals -rcd -ftz
WARNFLAGS= 

LD=$(CC) -static-intel
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm
