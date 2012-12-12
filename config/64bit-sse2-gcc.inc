# -*- makefile -*-
# configuration for Linux 64-bit x86 with GCC using SSE2
CC=gcc -m64
CPPFLAGS=
ARCHFLAGS= -march=opteron -mpc64 -msse2
GENFLAGS=  -falign-functions=16 -fPIC -fno-builtin
OPTFLAGS=  -O3 -ffast-math -fexpensive-optimizations
WARNFLAGS= -Wall -W

LD=$(CC)
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm

