# -*- makefile -*-
# configuration for Linux 32-bit x86 with GCC using the regular FPU
ALIGNMENT=16
CC=gcc -m32
CPPFLAGS=
ARCHFLAGS= -march=i686 -mpc64 -mfpmath=387 -g
GENFLAGS=  -falign-functions=$(ALIGNMENT) -fPIC -fno-builtin
OPTFLAGS=  -O -g
WARNFLAGS= -Wall -W

LD=$(CC)
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm
