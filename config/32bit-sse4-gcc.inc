# -*- makefile -*-
# configuration for Linux 32-bit x86 with GCC
ALIGNMENT=16
CC=gcc -m32
CPPFLAGS=
ARCHFLAGS= -march=core2 -mpc64 -msse4 -mfpmath=sse -g
GENFLAGS=  -falign-functions=$(ALIGNMENT) -fPIC -fno-builtin
OPTFLAGS=  -O3 -ffast-math -fexpensive-optimizations
WARNFLAGS= -Wall -W

LD=$(CC)
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm

