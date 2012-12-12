# -*- makefile -*-
# configuration for Linux 64-bit x86 with GCC with low optimization
CC=gcc -m64
CPPFLAGS=
ARCHFLAGS= -march=opteron -mpc64
GENFLAGS=  -falign-functions=16 -fPIC -fno-builtin
OPTFLAGS=  -O -g
WARNFLAGS= -Wall -W

LD=$(CC)
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm
