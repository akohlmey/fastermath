# -*- makefile -*-
# configuration for Linux 64-bit x86 with intel icc using SSE2
CC=icc
CPPFLAGS= 
ARCHFLAGS= -march=pentium3 -pc64 
GENFLAGS=  -falign-functions=16 -fPIC -fno-builtin
OPTFLAGS=  -O3 -ansi-alias -no-prec-div 
WARNFLAGS= 

LD=$(CC) -static-intel
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm
