# -*- makefile -*-
# configuration for Linux 64-bit x86 with GCC with low optimization
ALIGNMENT=16
CC=gcc -m64
CPPFLAGS=
ARCHFLAGS= -march=opteron -mpc64 -g
GENFLAGS=  -falign-functions=$(ALIGNMENT) -fPIC -fno-builtin
OPTFLAGS=  -O
WARNFLAGS= -Wall -W

LD=$(CC)
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm
