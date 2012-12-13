# -*- makefile -*-
# configuration for Linux 32-bit x86 with intel icc using SSE2
CC=icc -m32
CPPFLAGS= 
ARCHFLAGS= -mia32 -pc64 
GENFLAGS=  -falign-functions=16 -fPIC -fno-builtin
OPTFLAGS=  -O0 
WARNFLAGS= -Wall

LD=$(CC)
LDFLAGS=   -shared -static-intel
LDLIBS=
TESTLIBS=  -lrt -lm
