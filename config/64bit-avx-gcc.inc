# -*- makefile -*-
# configuration for Linux 64-bit x86 with GCC
CC=gcc
ARCHFLAGS=-m64 -march=core2 -mpc64 -mavx
CPPFLAGS=
LDLIBS= -lm
