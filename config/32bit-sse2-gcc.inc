# -*- makefile -*-
# configuration for Linux 32-bit x86 with GCC
CC=gcc
ARCHFLAGS=-m32 -march=pentium-m -msse2 -mpc64
CPPFLAGS=
LDLIBS= -lm
