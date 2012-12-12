# -*- makefile -*-
# configuration for Linux 64-bit x86 with GCC using the regular FPU
CC=gcc -m64
CPPFLAGS=
ARCHFLAGS= -march=opteron -mfpmath=387 -mpc64
GENFLAGS=  -falign-functions=16 -fPIC -fno-builtin
OPTFLAGS=  -O3 -ffast-math -fexpensive-optimizations
WARNFLAGS= -Wall -W

LD=$(CC)
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm
