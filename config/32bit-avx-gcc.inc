# -*- makefile -*-
# configuration for Linux 32-bit x86 with GCC
CC=gcc
ARCHFLAGS=-m32 -march=core2 -mpc64 -mavx
CPPFLAGS=
LDLIBS= -lm
