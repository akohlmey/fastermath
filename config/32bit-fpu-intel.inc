# -*- makefile -*-
# configuration for Linux 32-bit x86 with intel icc using SSE2
ALIGNMENT=16
CC=icc -m32
CPPFLAGS= 
ARCHFLAGS= -mia32 -pc64 -g
GENFLAGS=  -falign-functions=$(ALIGNMENT) -fPIC -fno-builtin
OPTFLAGS=  -O3 -ansi-alias -no-prec-div 
WARNFLAGS= 

LD=$(CC) -static-intel
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm
