# -*- makefile -*-
# configuration for Linux 32-bit x86 with GCC using the regular FPU
CC=gcc
ARCHFLAGS=-m32 -march=686 -mfpmath=387 -mpc64
OPTFLAGS=-O3 -ffast-math -fexpensive-optimizations
CPPFLAGS=
LDLIBS= -lm
