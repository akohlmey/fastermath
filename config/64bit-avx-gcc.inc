# -*- makefile -*-
# configuration for Linux 64-bit x86 with GCC
ALIGNMENT=32
CC=gcc -m64
CPPFLAGS=
ARCHFLAGS= -march=core2 -mpc64 -mavx
GENFLAGS=  -falign-functions=$(ALIGNMENT) -fPIC -fno-builtin
OPTFLAGS=  -O3 -ffast-math -fexpensive-optimizations
WARNFLAGS= -Wall -W

LD=$(CC)
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm
