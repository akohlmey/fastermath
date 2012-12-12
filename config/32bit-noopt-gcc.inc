# -*- makefile -*-
# configuration for Linux 32-bit x86 with GCC using the regular FPU
CC=gcc
CPPFLAGS=
ARCHFLAGS=-m32 -march=686 -mfpmath=387 -mpc64
GENFLAGS=-falign-functions=16 -fPIC -fno-builtins
OPTFLAGS=-O -g
WARNFLAGS=-Wall -W

LD=$(CC)
LDFLAGS=-shared
LDLIBS= -lm
