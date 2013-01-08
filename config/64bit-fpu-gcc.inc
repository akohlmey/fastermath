# -*- makefile -*-
# configuration for Linux 64-bit x86 with GCC using the regular FPU
ALIGNMENT=16
CC=gcc -m64
CPPFLAGS=
ARCHFLAGS= -march=opteron -mfpmath=387 -mpc64 -g
GENFLAGS=  -falign-functions=$(ALIGNMENT) -fPIC -fno-builtin
OPTFLAGS=  -O3 -ffast-math -fexpensive-optimizations
WARNFLAGS= -Wall -W

LD=$(CC)
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm
