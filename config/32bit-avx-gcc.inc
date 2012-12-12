# -*- makefile -*-
# configuration for Linux 32-bit x86 with GCC
CC=gcc -m32
CPPFLAGS=
ARCHFLAGS= -march=core2 -mpc64 -mavx
GENFLAGS=-falign-functions=16 -fPIC -fno-builtin
OPTFLAGS=-O -g
WARNFLAGS=-Wall -W

LD=$(CC)
LDFLAGS=-shared
LDLIBS=

TESTLIBS= -lrt -lm
