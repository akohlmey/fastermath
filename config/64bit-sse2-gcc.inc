# -*- makefile -*-
# configuration for Linux 64-bit x86 with GCC
CC=gcc
ARCHFLAGS=-m64 -march=opteron -msse2 -mpc64
CPPFLAGS=
LDLIBS= -lm
