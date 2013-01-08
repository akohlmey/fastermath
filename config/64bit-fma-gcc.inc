# -*- makefile -*-
# configuration for Linux 64-bit x86 with GCC
ALIGNMENT=16
CC=gcc -m64
CPPFLAGS=
ARCHFLAGS= -march=barcelona -mpc64 -mavx -mfma -g
GENFLAGS=  -falign-functions=$(ALIGNMENT) -fPIC -fno-builtin
OPTFLAGS=  -O3 -ffast-math -fexpensive-optimizations
WARNFLAGS= -Wall -W

LD=$(CC)
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm
