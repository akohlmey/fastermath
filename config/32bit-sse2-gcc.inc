# -*- makefile -*-
# configuration for Linux 32-bit x86 with GCC using SSE2
ALIGNMENT=16
CC=gcc -m32
CPPFLAGS=
ARCHFLAGS= -march=pentium-m -mpc64 -msse2 -mfpmath=sse -g
GENFLAGS=  -falign-functions=$(ALIGNMENT) -fPIC -fno-builtin
OPTFLAGS=  -O3 -ffast-math -fexpensive-optimizations
WARNFLAGS= -Wall -W

LD=$(CC)
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm
